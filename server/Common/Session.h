
// #ifndef RING_BUFFER_H
// #define RING_BUFFER_H

// #include <vector>
// #include <mutex>
// #include <atomic>
// #include <string>
// #include <google/protobuf/message.h>

// class SessionRingBuffer {
// public:
//     explicit SessionRingBuffer(size_t capacity);
//     ~SessionRingBuffer();

//     // 写入数据到缓冲区
//     bool push(const char* data, size_t length);
    
//     // 从缓冲区读取数据并反序列化为Protobuf对象
//     google::protobuf::Message* popAndDeserialize(const std::string& messageType);
    
//     // 获取可读数据大小
//     size_t readableSize() const;
    
//     // 获取空闲空间大小
//     size_t freeSize() const;
    
//     // 清空缓冲区
//     void clear();

// private:
//     std::vector<char> buffer_;
//     size_t capacity_;
//     std::atomic<size_t> read_pos_;
//     std::atomic<size_t> write_pos_;
//     std::mutex mutex_;
    
//     // 辅助方法
//     size_t getContinuousReadSize() const;
//     size_t getContinuousWriteSize() const;
// };


// struct MessageHeader
// {
//     int m_ID;
//     int m_Len;
// };
// class Seassion
// {
// public:


// private:
//     SessionRingBuffer m_rBuff;
//     SessionRingBuffer m_wBuff;
// };

// #endif
