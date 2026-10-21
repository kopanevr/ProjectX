# Eye Retail Project

**Целью данного проекта является разработка работающего прототипа устройства (MVP) для проверки ключевых гипотез, в связи с чем строгое следование формальным стандартам проектирования на данном этапе не предусмотрено.**

**Проект находится на стыке сфер розничной торговли среднего формата и технологий компьютерного зрения.**


**Вдохновением для проекта стал известный проект [Amazon Go](https://www.aboutamazon.com/news/company-news/amazon-fresh-go-stores-closing-expanding-whole-foods).**

## English

**Retail-focused computer vision MVP project.**

**Inspired by [Amazon Go](https://www.aboutamazon.com/news/company-news/amazon-fresh-go-stores-closing-expanding-whole-foods) project.**

## Зависимости

Для сборки и запуска проекта:

* **Компилятор C++:** С поддержкой C++20 и выше.
* **Python:**
* **OpenCV:** Для захвата видео (Версия 4).
* **TensorRT:** Для вывода модели.
* **Система сборки:** CMake (Версия 3.10).

## Железо (Hardware)

* **Вычислитель:** [NVIDIA Jetson Orin Nano](https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetson-orin/nano-super-developer-kit)

* **Камера:** [Logitech ]()

# Результат

**Пример лога при успешном запуске пайплайна счетчика клиентов:**

```text
[ИНФО] [Pipeline] Инициализация C++ ядра...
[ИНФО] [Inference Engine] Загрузка TensorRT движка: ../../../models/yolo26s.engine
[ИНФО] [Inference Engine] Имя входа: images
[ИНФО] [Inference Engine] Имя выхода: output0
[ИНФО] [Inference Engine] Запуск десериализации движка...
[ИНФО] [Inference Engine] Размер файла движка: 219626764 байт.
```

## Описание

Для обнаружения объектов применяется модель [YOLO26s](https://huggingface.co/Ultralytics/YOLO26).

Для проверки работоспособности сервера и корректности записи чеков выполните POST-запрос с помощью утилиты curl.

**Шаблон запроса:**

```bash
curl -X POST "http://X.X.X.X:X/receipts" \
    -H "Content-type: application/json" \
    -d '{"receipt_id": "X", "total_revenue": X}'
```

Для проверки работоспособности и корректности отправки данных на удаленный сервер выполните запуск скрипта [test.py](scripts/test.py).

**Шаблон :**

```bash


```

Устройство с периодичностью 5 минут производит отправку агрегированных данных на удаленный сервер.

**Пример данных передаваемых на удаленный сервер:**

```json
{
    "meta": {
        "version": 1,
        "subversion": 0
    },
    "device_id": 0,
    "interval_start": "2026-10-21T09:00:00Z",
    "interval_end": "2026-10-21T09:05:00Z",
    "data": {
        "visitors_in": 1,
        "visitors_out": 1,
        "receipts_count": 0,
        "total_revenue": 5000.0
    },
    "diagnostic": {
        "device_state": "OK"
    }
}
```

##
