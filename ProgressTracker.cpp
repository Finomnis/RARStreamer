#include "ProgressTracker.h"



void ProgressTracker::addNewFile(uint64_t size)
{
    pastFilesSize += currentFileSize;
    currentFileSize = size;
}

void ProgressTracker::addExtractedData(uint64_t size)
{
    totalExtractedData += size;
}

float ProgressTracker::getCurrentFilePercent() const
{
    uint64_t currentFileExtracted = totalExtractedData - pastFilesSize;
    return 100.0f * float(currentFileExtracted) / float(currentFileSize);
}
