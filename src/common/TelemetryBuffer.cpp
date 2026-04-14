
#include "project/common/TelemetryBuffer.hpp"

namespace project::common
{
    TelemetryBuffer::TelemetryBuffer(std::size_t capacity)
        : capacity_(capacity)
    {
        data_.reserve(capacity_);
    }

    void TelemetryBuffer::push(const TelemetryPoint &point)
    {
        if (data_.size() < capacity_)
        {
            data_.push_back(point);
        }
        else
        {
            data_[head_] = point;
            head_ = (head_ + 1) % capacity_;
            wrapped_ = true;
        }
    }

    std::vector<TelemetryPoint> TelemetryBuffer::snapshot() const
    {
        if (!wrapped_)
        {
            return data_;
        }

        std::vector<TelemetryPoint> snapshot;
        snapshot.reserve(capacity_);
        for (std::size_t i = 0; i < capacity_; ++i)
        {
            std::size_t index = (head_ + i) % capacity_;
            snapshot.push_back(data_[index]);
        }
        return snapshot;
    }

    void TelemetryBuffer::clear()
    {
        data_.clear();
        head_ = 0;
        wrapped_ = false;
    }
}