#pragma once

#include <cstdint>

class ProgressTracker
{
    public:
        void addNewFile(uint64_t size);
        void addExtractedData(uint64_t size);
        float getCurrentFilePercent() const;

    private:
        uint64_t totalExtractedData = 0;
        uint64_t currentFileSize = 0;
        uint64_t pastFilesSize = 0;

};
