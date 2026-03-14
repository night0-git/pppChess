#include "BoardView.hpp"
#include <iostream>
#include <cmath>
using ui::BoardView;

BoardView::BoardView(const ResourceManager<TextureId, sf::Texture>& textures, SoundPlayer& sounds, const Board& board)
: _textures(textures), _sounds(sounds), _board(board) {
    // Set origin the the center
    setOrigin({getSize().x / 2, getSize().y / 2});
}

void BoardView::handleEvent(const sf::Event& event, const sf::RenderWindow& window, sf::Vector2f mouseWorldPos) {
    sf::Vector2f localPos = getTransform().getInverse().transformPoint(mouseWorldPos);

    const sf::Vector2i sqr = localPosToSqr(localPos);
    bool isHovered = _board.isWithinBoard(sqr) && _board.getPieceAt(sqr);

    if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        if (isHovered) {
            _state = ui::State::Pressed;
        }
        if (_board.isWithinBoard(sqr)) {
            // Handle click move first
            if (_selectedSqr) {
                // Check piece selection
                if (*_selectedSqr == sqr && !_isDeselecting) {
                    _isDeselecting = true;
                }
                _isMoving = true;
                if (_onMoveRequest && _onMoveRequest({*_selectedSqr, sqr})) {
                    _selectedSqr = std::nullopt;
                    return;
                }
                _isMoving = false;
            }
            auto pcs = _board.getPieceAt(sqr);
            if (pcs) {
                _selectedSqr = sqr;
                _draggedPiece = pcs;
                _pieceViews.at(pcs).setPosition(localPos);
            }
            else if (_selectedSqr) {
                _selectedSqr = std::nullopt;
            }
        }
    }

    else if (event.is<sf::Event::MouseMoved>()) {
        if (_draggedPiece) {
            _pieceViews.at(_draggedPiece).setPosition(localPos);
        }
    }

    else if (event.is<sf::Event::MouseButtonReleased>() && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
        if (_state == ui::State::Pressed) {
            _state = isHovered ? ui::State::Hovered : ui::State::Idle;
        }
        if (_draggedPiece && _selectedSqr) {
            // Check piece deselection
            if (*_selectedSqr == sqr && _isDeselecting) {
                _pieceViews.at(_draggedPiece).snapToSquare(*_selectedSqr);
                _selectedSqr = std::nullopt;
                _isDeselecting = false;
            }
            else {
                bool moved = _onMoveRequest && _onMoveRequest({*_selectedSqr, sqr});
                if (!moved) {
                    // Snap back
                    _pieceViews.at(_draggedPiece).snapToSquare(*_selectedSqr);
                }
                else {
                    _selectedSqr = std::nullopt;
                    // Manually set because isHovered is not up to date
                    isHovered = true;
                }
            }
            _draggedPiece = nullptr;
        }
    }

    else if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right) {
        if (_selectedSqr && _draggedPiece) {
            _pieceViews.at(_draggedPiece).snapToSquare(*_selectedSqr);
            _isMoving = false;
            _draggedPiece = nullptr;
            _selectedSqr = std::nullopt;
            _isDeselecting = false;
            _state = ui::State::Idle;
        }
    }

    if (isHovered && _state == ui::State::Idle) {
        _state = ui::State::Hovered;
    } else if (!isHovered && _state == ui::State::Hovered) {
        _state = ui::State::Idle;
    }
}

void BoardView::update(sf::Time dt) {
    for (auto& [piece, view] : _pieceViews) {
        view.update(dt);
    }
}

sf::Vector2f BoardView::getSize() const {
    return sf::Vector2f(_theme.tileSize * Board::SIZE, _theme.tileSize * Board::SIZE);
}

void BoardView::setSize(sf::Vector2f size) {
    _theme.tileSize = size.x / Board::SIZE;
    // Set board origin the the center
    setOrigin({getSize().x / 2, getSize().y / 2});
    for (auto& [piece, view] : _pieceViews) {
        view.normalizeSprite();
    }
}

void BoardView::flip() {
    rotate(sf::degrees(180));
    for (auto& [pcs, pcsView] : _pieceViews) {
        pcsView.rotate(sf::degrees(180));
    }
}

void BoardView::onBoardInit() {
    _pieceViews.clear();
    for (int x = 0; x < Board::SIZE; x++) {
        for (int y = 0; y < Board::SIZE; y++) {
            auto pcs = _board.getPieceAt({x, y});
            if (pcs) {
                const sf::Texture& texture = _textures.get(ui::getPieceId(*pcs));
                // Use piecewise_construct for multi-argument constructors in maps
                _pieceViews.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(pcs),
                                    std::forward_as_tuple(texture, _theme.tileSize, *pcs));
                _pieceViews.at(pcs).snapToSquare({x, y});
            }
        }
    }
    _sounds.play(ui::SoundId::GameStart);
}

void BoardView::onMoveEvent(const MoveResult& result) {
    if (!result.success) {
        if (result.special == SpecialMove::Illegal) {
            _sounds.play(ui::SoundId::Illegal);
        }
        return;
    }

    // Delete old pieceview
    if (result.captured) {
        _pieceViews.erase(result.captured.get());
    }
    
    // Update pieceViews map if this is promotion
    if (result.promotedPawn) {
        _pieceViews.erase(result.promotedPawn.get());
        auto pcs = _board.getPieceAt(result.move.dest);
        if (pcs) {
            const sf::Texture& texture = _textures.get(ui::getPieceId(*pcs));
            PieceView pcsView(texture, _theme.tileSize, *pcs);
            pcsView.snapToSquare(result.move.dest);
            _pieceViews.insert_or_assign(pcs, pcsView);
            // Make sure the newly created piece is rotated correctly
            if (getRotation() == sf::degrees(180)) {
                _pieceViews.at(pcs).rotate(sf::degrees(180));
            }
        }
    }

    // Move pieceview
    if (result.rookMove) {
        movePieceView(*result.rookMove);
    }
    movePieceView(result.move);

    // Play sound
    if (result.isCheck) {
        _sounds.play(ui::SoundId::Check);
        return;
    }
    if (result.special == SpecialMove::None || result.special == SpecialMove::EnPassant) {
        ui::SoundId sound = result.captured ? ui::SoundId::Capture : ui::SoundId::Move;
        _sounds.play(sound);
    } else if (result.special == SpecialMove::Castle) {
        _sounds.play(ui::SoundId::Castle);
    } else if (result.special == SpecialMove::Promote) {
        _sounds.play(ui::SoundId::Promote);
    }
}

void BoardView::onPromoteSelection(sf::Vector2i sqr, PieceType& type) {
    // Display and choose promotion type here
    type = PieceType::Queen;
}

void BoardView::movePieceView(Move move) {
    if (auto pcs = _board.getPieceAt(move.dest)) {
        if (_isMoving) {
            _pieceViews.at(pcs).animateToPosition(move.dest);
            _isMoving = false;
        } else {
            _pieceViews.at(pcs).snapToSquare(move.dest);
        }
    }
}

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    // Draw grid;
    sf::RectangleShape square({_theme.tileSize, _theme.tileSize});
    for (int col = 0; col < Board::SIZE; col++) {
        for (int row = 0; row < Board::SIZE; row++) {
            square.setPosition({col * _theme.tileSize, row * _theme.tileSize});
            square.setFillColor((col + row) % 2 == 0 ? _theme.lightColor : _theme.darkColor);
            target.draw(square, states);
            if (_selectedSqr == sf::Vector2i(row, col)) {
                // Draw an overlay for selected square
                sf::Color yellow = {255, 235, 59, 100};
                square.setFillColor(yellow);
                target.draw(square, states);
            }
        }
    }
    // Draw piece
    for (const auto& [piece, view] : _pieceViews) {
        if (piece != _draggedPiece) {
            target.draw(view, states);
        }
    }
    if (_draggedPiece) {
        auto it = _pieceViews.find(_draggedPiece);
        if (it != _pieceViews.end()) {
           target.draw(_pieceViews.at(_draggedPiece), states);
        }
    }
}

sf::Vector2i BoardView::localPosToSqr(const sf::Vector2f& localPos) const {
    int col = static_cast<int>(std::floor(localPos.x / _theme.tileSize));
    int row = static_cast<int>(std::floor(localPos.y / _theme.tileSize));
    return sf::Vector2i(row, col);
}