#pragma once
#include "bobtypes.h"
#include "NDGameEngine.h"
#include "ND.h"
#include "src/Engine/Puzzle/GameLogic.h"
#include "src/Engine/Puzzle/PuzzlePlayer.h"
#include <memory>

class NDPuzzleGame : public NDGameEngine {
private:
    std::shared_ptr<GameLogic> puzzleGame;
    std::shared_ptr<PuzzlePlayer> puzzlePlayer;

public:
    NDPuzzleGame(ND* nd) : NDGameEngine(nd) {}

    void init() override {
        puzzleGame = std::make_shared<GameLogic>();
        puzzlePlayer = std::make_shared<PuzzlePlayer>(puzzleGame.get());
        puzzleGame->initGame();
        puzzleGame->start();
    }

    void cleanup() override {
        puzzleGame = nullptr;
        puzzlePlayer = nullptr;
    }

    void update(float dt) override {
        if (puzzleGame && puzzlePlayer) {
            // Map inputs here if needed
            puzzleGame->update();
        }
    }

    void render() override {
        if (puzzleGame) {
            // Render logic here
        }
    }

    void titleMenuUpdate() override {}
};
