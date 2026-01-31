
// #include "protobuf_deserializer.h"

// // 修复：使用const指针接收generated_pool()的返回值
// const google::protobuf::DescriptorPool* ProtobufDeserializer::descriptor_pool_ = 
//     google::protobuf::DescriptorPool::generated_pool();

// google::protobuf::DynamicMessageFactory* ProtobufDeserializer::message_factory_ = 
//     new google::protobuf::DynamicMessageFactory();

// google::protobuf::Message* ProtobufDeserializer::deserialize(const std::string& messageType, 
//                                                               const char* data, 
//                                                               size_t length) {
//     // 查找消息描述符
//     const google::protobuf::Descriptor* descriptor = 
//         descriptor_pool_->FindMessageTypeByName(messageType);
    
//     if (!descriptor) {
//         return nullptr;
//     }
    
//     // 创建消息原型
//     const google::protobuf::Message* prototype = 
//         message_factory_->GetPrototype(descriptor);
    
//     if (!prototype) {
//         return nullptr;
//     }
    
//     // 创建新消息实例
//     google::protobuf::Message* message = prototype->New();
//     if (!message) {
//         return nullptr;
//     }
    
//     // 解析数据
//     if (message->ParseFromArray(data, static_cast<int>(length))) {
//         return message;
//     } else {
//         delete message;
//         return nullptr;
//     }
// }

// bool ProtobufDeserializer::registerMessageType(const google::protobuf::Descriptor* descriptor) {
//     if (!descriptor) {
//         return false;
//     }
//     return true;
// }
