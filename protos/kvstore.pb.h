// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: kvstore.proto

#ifndef PROTOBUF_INCLUDED_kvstore_2eproto
#define PROTOBUF_INCLUDED_kvstore_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_kvstore_2eproto 

namespace protobuf_kvstore_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_kvstore_2eproto
namespace kvstore {
class CommandRequest;
class CommandRequestDefaultTypeInternal;
extern CommandRequestDefaultTypeInternal _CommandRequest_default_instance_;
class CommandResponse;
class CommandResponseDefaultTypeInternal;
extern CommandResponseDefaultTypeInternal _CommandResponse_default_instance_;
}  // namespace kvstore
namespace google {
namespace protobuf {
template<> ::kvstore::CommandRequest* Arena::CreateMaybeMessage<::kvstore::CommandRequest>(Arena*);
template<> ::kvstore::CommandResponse* Arena::CreateMaybeMessage<::kvstore::CommandResponse>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace kvstore {

enum OperationType {
  SET = 0,
  GET = 1,
  DELETE = 2,
  OperationType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  OperationType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool OperationType_IsValid(int value);
const OperationType OperationType_MIN = SET;
const OperationType OperationType_MAX = DELETE;
const int OperationType_ARRAYSIZE = OperationType_MAX + 1;

const ::google::protobuf::EnumDescriptor* OperationType_descriptor();
inline const ::std::string& OperationType_Name(OperationType value) {
  return ::google::protobuf::internal::NameOfEnum(
    OperationType_descriptor(), value);
}
inline bool OperationType_Parse(
    const ::std::string& name, OperationType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<OperationType>(
    OperationType_descriptor(), name, value);
}
enum ErrorCode {
  OK = 0,
  NOT_LEADER = 1,
  TIMEOUT = 2,
  NOT_FOUND = 3,
  ErrorCode_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  ErrorCode_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool ErrorCode_IsValid(int value);
const ErrorCode ErrorCode_MIN = OK;
const ErrorCode ErrorCode_MAX = NOT_FOUND;
const int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

const ::google::protobuf::EnumDescriptor* ErrorCode_descriptor();
inline const ::std::string& ErrorCode_Name(ErrorCode value) {
  return ::google::protobuf::internal::NameOfEnum(
    ErrorCode_descriptor(), value);
}
inline bool ErrorCode_Parse(
    const ::std::string& name, ErrorCode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ErrorCode>(
    ErrorCode_descriptor(), name, value);
}
// ===================================================================

class CommandRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:kvstore.CommandRequest) */ {
 public:
  CommandRequest();
  virtual ~CommandRequest();

  CommandRequest(const CommandRequest& from);

  inline CommandRequest& operator=(const CommandRequest& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  CommandRequest(CommandRequest&& from) noexcept
    : CommandRequest() {
    *this = ::std::move(from);
  }

  inline CommandRequest& operator=(CommandRequest&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const CommandRequest& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const CommandRequest* internal_default_instance() {
    return reinterpret_cast<const CommandRequest*>(
               &_CommandRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(CommandRequest* other);
  friend void swap(CommandRequest& a, CommandRequest& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline CommandRequest* New() const final {
    return CreateMaybeMessage<CommandRequest>(NULL);
  }

  CommandRequest* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<CommandRequest>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const CommandRequest& from);
  void MergeFrom(const CommandRequest& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(CommandRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string key = 1;
  void clear_key();
  static const int kKeyFieldNumber = 1;
  const ::std::string& key() const;
  void set_key(const ::std::string& value);
  #if LANG_CXX11
  void set_key(::std::string&& value);
  #endif
  void set_key(const char* value);
  void set_key(const char* value, size_t size);
  ::std::string* mutable_key();
  ::std::string* release_key();
  void set_allocated_key(::std::string* key);

  // string value = 2;
  void clear_value();
  static const int kValueFieldNumber = 2;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  #if LANG_CXX11
  void set_value(::std::string&& value);
  #endif
  void set_value(const char* value);
  void set_value(const char* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // .kvstore.OperationType type = 3;
  void clear_type();
  static const int kTypeFieldNumber = 3;
  ::kvstore::OperationType type() const;
  void set_type(::kvstore::OperationType value);

  // @@protoc_insertion_point(class_scope:kvstore.CommandRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr key_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  int type_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_kvstore_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class CommandResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:kvstore.CommandResponse) */ {
 public:
  CommandResponse();
  virtual ~CommandResponse();

  CommandResponse(const CommandResponse& from);

  inline CommandResponse& operator=(const CommandResponse& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  CommandResponse(CommandResponse&& from) noexcept
    : CommandResponse() {
    *this = ::std::move(from);
  }

  inline CommandResponse& operator=(CommandResponse&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const CommandResponse& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const CommandResponse* internal_default_instance() {
    return reinterpret_cast<const CommandResponse*>(
               &_CommandResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(CommandResponse* other);
  friend void swap(CommandResponse& a, CommandResponse& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline CommandResponse* New() const final {
    return CreateMaybeMessage<CommandResponse>(NULL);
  }

  CommandResponse* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<CommandResponse>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const CommandResponse& from);
  void MergeFrom(const CommandResponse& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(CommandResponse* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string value = 2;
  void clear_value();
  static const int kValueFieldNumber = 2;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  #if LANG_CXX11
  void set_value(::std::string&& value);
  #endif
  void set_value(const char* value);
  void set_value(const char* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // string leaderHint = 3;
  void clear_leaderhint();
  static const int kLeaderHintFieldNumber = 3;
  const ::std::string& leaderhint() const;
  void set_leaderhint(const ::std::string& value);
  #if LANG_CXX11
  void set_leaderhint(::std::string&& value);
  #endif
  void set_leaderhint(const char* value);
  void set_leaderhint(const char* value, size_t size);
  ::std::string* mutable_leaderhint();
  ::std::string* release_leaderhint();
  void set_allocated_leaderhint(::std::string* leaderhint);

  // bool success = 1;
  void clear_success();
  static const int kSuccessFieldNumber = 1;
  bool success() const;
  void set_success(bool value);

  // .kvstore.ErrorCode errorCode = 4;
  void clear_errorcode();
  static const int kErrorCodeFieldNumber = 4;
  ::kvstore::ErrorCode errorcode() const;
  void set_errorcode(::kvstore::ErrorCode value);

  // @@protoc_insertion_point(class_scope:kvstore.CommandResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  ::google::protobuf::internal::ArenaStringPtr leaderhint_;
  bool success_;
  int errorcode_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_kvstore_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// CommandRequest

// string key = 1;
inline void CommandRequest::clear_key() {
  key_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CommandRequest::key() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandRequest.key)
  return key_.GetNoArena();
}
inline void CommandRequest::set_key(const ::std::string& value) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:kvstore.CommandRequest.key)
}
#if LANG_CXX11
inline void CommandRequest::set_key(::std::string&& value) {
  
  key_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:kvstore.CommandRequest.key)
}
#endif
inline void CommandRequest::set_key(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:kvstore.CommandRequest.key)
}
inline void CommandRequest::set_key(const char* value, size_t size) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:kvstore.CommandRequest.key)
}
inline ::std::string* CommandRequest::mutable_key() {
  
  // @@protoc_insertion_point(field_mutable:kvstore.CommandRequest.key)
  return key_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CommandRequest::release_key() {
  // @@protoc_insertion_point(field_release:kvstore.CommandRequest.key)
  
  return key_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CommandRequest::set_allocated_key(::std::string* key) {
  if (key != NULL) {
    
  } else {
    
  }
  key_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), key);
  // @@protoc_insertion_point(field_set_allocated:kvstore.CommandRequest.key)
}

// string value = 2;
inline void CommandRequest::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CommandRequest::value() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandRequest.value)
  return value_.GetNoArena();
}
inline void CommandRequest::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:kvstore.CommandRequest.value)
}
#if LANG_CXX11
inline void CommandRequest::set_value(::std::string&& value) {
  
  value_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:kvstore.CommandRequest.value)
}
#endif
inline void CommandRequest::set_value(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:kvstore.CommandRequest.value)
}
inline void CommandRequest::set_value(const char* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:kvstore.CommandRequest.value)
}
inline ::std::string* CommandRequest::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:kvstore.CommandRequest.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CommandRequest::release_value() {
  // @@protoc_insertion_point(field_release:kvstore.CommandRequest.value)
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CommandRequest::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:kvstore.CommandRequest.value)
}

// .kvstore.OperationType type = 3;
inline void CommandRequest::clear_type() {
  type_ = 0;
}
inline ::kvstore::OperationType CommandRequest::type() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandRequest.type)
  return static_cast< ::kvstore::OperationType >(type_);
}
inline void CommandRequest::set_type(::kvstore::OperationType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:kvstore.CommandRequest.type)
}

// -------------------------------------------------------------------

// CommandResponse

// bool success = 1;
inline void CommandResponse::clear_success() {
  success_ = false;
}
inline bool CommandResponse::success() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandResponse.success)
  return success_;
}
inline void CommandResponse::set_success(bool value) {
  
  success_ = value;
  // @@protoc_insertion_point(field_set:kvstore.CommandResponse.success)
}

// string value = 2;
inline void CommandResponse::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CommandResponse::value() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandResponse.value)
  return value_.GetNoArena();
}
inline void CommandResponse::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:kvstore.CommandResponse.value)
}
#if LANG_CXX11
inline void CommandResponse::set_value(::std::string&& value) {
  
  value_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:kvstore.CommandResponse.value)
}
#endif
inline void CommandResponse::set_value(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:kvstore.CommandResponse.value)
}
inline void CommandResponse::set_value(const char* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:kvstore.CommandResponse.value)
}
inline ::std::string* CommandResponse::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:kvstore.CommandResponse.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CommandResponse::release_value() {
  // @@protoc_insertion_point(field_release:kvstore.CommandResponse.value)
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CommandResponse::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:kvstore.CommandResponse.value)
}

// string leaderHint = 3;
inline void CommandResponse::clear_leaderhint() {
  leaderhint_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CommandResponse::leaderhint() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandResponse.leaderHint)
  return leaderhint_.GetNoArena();
}
inline void CommandResponse::set_leaderhint(const ::std::string& value) {
  
  leaderhint_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:kvstore.CommandResponse.leaderHint)
}
#if LANG_CXX11
inline void CommandResponse::set_leaderhint(::std::string&& value) {
  
  leaderhint_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:kvstore.CommandResponse.leaderHint)
}
#endif
inline void CommandResponse::set_leaderhint(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  leaderhint_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:kvstore.CommandResponse.leaderHint)
}
inline void CommandResponse::set_leaderhint(const char* value, size_t size) {
  
  leaderhint_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:kvstore.CommandResponse.leaderHint)
}
inline ::std::string* CommandResponse::mutable_leaderhint() {
  
  // @@protoc_insertion_point(field_mutable:kvstore.CommandResponse.leaderHint)
  return leaderhint_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CommandResponse::release_leaderhint() {
  // @@protoc_insertion_point(field_release:kvstore.CommandResponse.leaderHint)
  
  return leaderhint_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CommandResponse::set_allocated_leaderhint(::std::string* leaderhint) {
  if (leaderhint != NULL) {
    
  } else {
    
  }
  leaderhint_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), leaderhint);
  // @@protoc_insertion_point(field_set_allocated:kvstore.CommandResponse.leaderHint)
}

// .kvstore.ErrorCode errorCode = 4;
inline void CommandResponse::clear_errorcode() {
  errorcode_ = 0;
}
inline ::kvstore::ErrorCode CommandResponse::errorcode() const {
  // @@protoc_insertion_point(field_get:kvstore.CommandResponse.errorCode)
  return static_cast< ::kvstore::ErrorCode >(errorcode_);
}
inline void CommandResponse::set_errorcode(::kvstore::ErrorCode value) {
  
  errorcode_ = value;
  // @@protoc_insertion_point(field_set:kvstore.CommandResponse.errorCode)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace kvstore

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::kvstore::OperationType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::kvstore::OperationType>() {
  return ::kvstore::OperationType_descriptor();
}
template <> struct is_proto_enum< ::kvstore::ErrorCode> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::kvstore::ErrorCode>() {
  return ::kvstore::ErrorCode_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_kvstore_2eproto
