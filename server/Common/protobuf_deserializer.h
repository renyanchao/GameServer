
#ifndef PROTOBUF_DESERIALIZER_H
#define PROTOBUF_DESERIALIZER_H

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <string>

class ProtobufDeserializer {
public:
    // 动态反序列化方法
    static google::protobuf::Message* deserialize(const std::string& messageType, 
                                                   const char* data, 
                                                   size_t length);
    
    // 注册消息类型
    static bool registerMessageType(const google::protobuf::Descriptor* descriptor);

private:
    static const google::protobuf::DescriptorPool* descriptor_pool_;
    static google::protobuf::DynamicMessageFactory* message_factory_;
};

#endif
