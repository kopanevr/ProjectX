#pragma once

//

#include <cassert>
#include <cstdint>
#include <cstring>

//

using bitSubfield_t = uint64_t;

//

/// @brief Флаг-состояние.
/// @param name Имя флага-состояния.
#define FLAG(name) bitSubfield_t name : 1;

/// @brief Флаг-состояние .
/// @param name Имя флага-состояния.
/// @brief state Состояние.
#define FLAG_WITH_DEFAULT_STATE(name, state) bitSubfield_t name : 1 {state};

/// @brief Битовое подполе.
/// @param name Имя битового подполя.
/// @param size Размер битового подполя.
#define SUBFIELD(name, size)                                                   \
  bitSubfield_t name : (static_assert(size <= sizeof(bitSubfield_t) * 8));

#define MAKE_BIT_FIELD_NAME(id) bitField_##id
#define GET_BIT_FIELD_NAME(id) bitField_##id

/// @brief Битовое пое с ожидаемым размером.
/// @param id Идентификатор.
/// @param expectedSize Ожидаемый размер в байт.
/// @param listOfBitSubfield Список флагов-состояний или битовых подполей.
#define BIT_FIELD(id, expectedSize, listOfBitSubfield)                         \
  struct __attribute__((packed)) {                                             \
    union {                                                                    \
      struct __attribute__((packed)) {                                         \
        listOfBitSubfield                                                      \
      };                                                                       \
      uint8_t temp[expectedSize];                                              \
    };                                                                         \
  } MAKE_BIT_FIELD_NAME(id);                                                   \
  static_assert(sizeof(GET_BIT_FIELD_NAME(id)) == expectedSize)

/// @brief Статическое битовое поле с ожидаемым размером.
/// @param id Идентификатор.
/// @param expectedSize Ожидаемый размер в байт.
/// @param listOfBitSubfield Список флагов-состояний или битовых подполей.
#define STATIC_BIT_FIELD(id, expectedSize, listOfBitSubfield)                  \
  static struct __attribute__((packed)) {                                      \
    union {                                                                    \
      struct __attribute__((packed)) {                                         \
        listOfBitSubfield                                                      \
      };                                                                       \
      uint8_t temp[expectedSize];                                              \
    };                                                                         \
  } MAKE_BIT_FIELD_NAME(id);                                                   \
  static_assert(sizeof(GET_BIT_FIELD_NAME(id)) == expectedSize)

/// @brief Константное битовое поле с ожидаемым размером.
/// @param id Идентификатор.
/// @param expectedSize Ожидаемый размер в байт.
/// @param listOfBitSubfield Список флагов-состояний или битовых подполей.
#define CONST_BIT_FIELD(id, expectedSize, listOfBitSubfield)                   \
  const struct __attribute__((packed)) {                                       \
    union {                                                                    \
      struct __attribute__((packed)) {                                         \
        listOfBitSubfield                                                      \
      };                                                                       \
      uint8_t temp[expectedSize];                                              \
    };                                                                         \
  } MAKE_BIT_FIELD_NAME(id);                                                   \
  static_assert(sizeof(GET_BIT_FIELD_NAME(id)) == expectedSize)

/// @brief Возвращает битовое поле по идентификатору.
/// @param id Идентификатор.
#define GET_BIT_FIELD(id) GET_BIT_FIELD_NAME(id)

/// @brief Устанавливает 1 во все биты битового поля по идентификатору.
/// @param id Идентификатор.
#define SET_BIT_FIELD(id)                                                      \
  std::memset(static_cast<void *>(&GET_BIT_FIELD_NAME(id)),                    \
              0xFF /* 0b11111111 */, sizeof(GET_BIT_FIELD_NAME(id)))

/// @brief Стирает битовое поле по идентификатору.
/// @param id Идентификатор.
#define ERASE_BIT_FIELD(id)                                                    \
  std::memset(static_cast<void *>(&GET_BIT_FIELD_NAME(id)), 0,                 \
              sizeof(GET_BIT_FIELD_NAME(id)))

/// @brief Устанавливает флаг.
#define SET_FLAG(id, flag) GET_BIT_FIELD_NAME(id).flag = 1
/// @brief Сбрасывает флаг.
#define RESET_FLAG(id, flag) GET_BIT_FIELD_NAME(id).flag = 0
/// @brief Получает состояние флага.
#define GET_FLAG_STATE(id, flag) (GET_BIT_FIELD_NAME(id).flag & 1)
