#include "Session.h"
#include "protobuf_deserializer.h"
#include <cstring>

SessionRingBuffer::SessionRingBuffer(size_t capacity) 
    : capacity_(capacity), read_pos_(0), write_pos_(0) {
    buffer_.resize(capacity);
}

SessionRingBuffer::~SessionRingBuffer() {
    clear();
}

bool SessionRingBuffer::push(const char* data, size_t length) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (length > freeSize()) {
        return false; // 空间不足
    }
    
    // 处理环绕写入
    size_t continuous_write = getContinuousWriteSize();
    if (length <= continuous_write) {
        std::memcpy(&buffer_[write_pos_], data, length);
        write_pos_ = (write_pos_ + length) % capacity_;
    } else {
        std::memcpy(&buffer_[write_pos_], data, continuous_write);
        std::memcpy(&buffer_[0], data + continuous_write, length - continuous_write);
        write_pos_ = length - continuous_write;
    }
    
    return true;
}

google::protobuf::Message* SessionRingBuffer::popAndDeserialize(const std::string& messageType) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t readable = readableSize();
    if (readable < sizeof(uint32_t)) {
        return nullptr; // 数据不足以读取消息长度
    }
    
    // 读取消息长度
    uint32_t msg_length;
    size_t continuous_read = getContinuousReadSize();
    
    if (sizeof(msg_length) <= continuous_read) {
        std::memcpy(&msg_length, &buffer_[read_pos_], sizeof(msg_length));
    } else {
        std::memcpy(&msg_length, &buffer_[read_pos_], continuous_read);
        std::memcpy(reinterpret_cast<char*>(&msg_length) + continuous_read, 
                     &buffer_[0], sizeof(msg_length) - continuous_read);
    }
    
    if (msg_length > readable - sizeof(msg_length)) {
        return nullptr; // 数据不完整
    }
    
    // 提取消息数据
    std::vector<char> message_data(msg_length);
    size_t data_start_pos = (read_pos_ + sizeof(msg_length)) % capacity_;
    size_t continuous_data_read = capacity_ - data_start_pos;
    
    if (msg_length <= continuous_data_read) {
        std::memcpy(message_data.data(), &buffer_[data_start_pos], msg_length);
    } else {
        std::memcpy(message_data.data(), &buffer_[data_start_pos], continuous_data_read);
        std::memcpy(message_data.data() + continuous_data_read, 
                     &buffer_[0], msg_length - continuous_data_read);
    }
    
    // 更新读位置
    read_pos_ = (read_pos_ + sizeof(msg_length) + msg_length) % capacity_;
    
    // 动态反序列化
    return ProtobufDeserializer::deserialize(messageType, message_data.data(), msg_length);
}

size_t SessionRingBuffer::readableSize() const {
    size_t write_pos = write_pos_.load();
    size_t read_pos = read_pos_.load();
    
    if (write_pos >= read_pos) {
        return write_pos - read_pos;
    } else {
        return capacity_ - read_pos + write_pos;
    }
}

size_t SessionRingBuffer::freeSize() const {
    return capacity_ - readableSize() - 1; // 保留一个位置避免读写指针重合
}

size_t SessionRingBuffer::getContinuousReadSize() const {
    size_t write_pos = write_pos_.load();
    size_t read_pos = read_pos_.load();
    
    if (write_pos >= read_pos) {
        return write_pos - read_pos;
    } else {
        return capacity_ - read_pos;
    }
}

size_t SessionRingBuffer::getContinuousWriteSize() const {
    size_t write_pos = write_pos_.load();
    size_t read_pos = read_pos_.load();
    
    if (write_pos >= read_pos) {
        return capacity_ - write_pos + (read_pos > 0 ? 0 : 1);
    } else {
        return read_pos - write_pos - 1;
    }
}

void SessionRingBuffer::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    read_pos_ = 0;
    write_pos_ = 0;
}




#include"Base.pb.h"

void test()
{
    // MyMessage msg;
    // msg.GetTypeName();
}