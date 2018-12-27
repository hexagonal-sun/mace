#pragma once

#include "board.h"
#include "move.hpp"
#include <chrono>
#include <functional>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

enum class MinMax {
    MIN,
    MAX
};

class SearchResults
{
    size_t depth_;
    // moveList_t pv_;
    Move bestMove_;
    int score_;
    size_t nodes_;
    TimePoint start_, end_;
    MinMax searchDir_;

public:
    SearchResults(size_t depthToBeSearched, MinMax searchDir)
        : depth_(depthToBeSearched), nodes_(0),
          start_(Clock::now()), searchDir_(searchDir)
        {
            if (searchDir_ == MinMax::MIN)
                score_ = INT_MAX;
            else
                score_ = -INT_MAX;
        }

    void finishedSearch()
        {
            end_ = Clock::now();
        }

    size_t getDepth() const
        {
            return depth_;
        }

    int getScore() const
        {
            return score_;
        }

    size_t getNodes() const
        {
            return nodes_;
        }

    size_t getNPS() const
        {
            std::chrono::duration<double> duration = end_ - start_;
            return static_cast<double>(nodes_) / duration.count();
        }

    void vistedNode()
        {
            nodes_++;
        }

    Move getBestMove() const
        {
            return bestMove_;
        }

    void setBestMove(Move m)
        {
            bestMove_ = m;
        }

    // moveList_t getPV() const
    //     {
    //         return pv_;
    //     }

    // void setPV(const moveList_t &moves)
    //     {
    //         pv_ = moves;
    //     }

    MinMax getSearchDir() const
        {
            return searchDir_;
        }

    void setScore(int newScore)
        {
            score_ = newScore;
        }

    template<typename Tdur>
    size_t getDuration() const
        {
            auto duration = std::chrono::duration_cast<Tdur> (end_ - start_);
            return duration.count();
        }
};

// Search for a best position for the given time limit (in ms).
Move searchMove(Board &b, const Clock::duration timeLimit,
                std::function<void(SearchResults &results)> dumpResults);
