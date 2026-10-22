# Eye Retail Project

**Проект находится на стыке сфер розничной торговли среднего формата и технологий компьютерного зрения.**

**Вдохновением для проекта послужил известный проект [Amazon Go](https://www.aboutamazon.com/news/company-news/amazon-fresh-go-stores-closing-expanding-whole-foods).**

### English

Retail-focused computer vision MVP project.

Inspired by [Amazon Go](https://www.aboutamazon.com/news/company-news/amazon-fresh-go-stores-closing-expanding-whole-foods) project.

## Цель (Goal)

**Оптимизировать взаимодействие менеджмента с аналитикой заведения для среднего формата, открыв возможность оперативно отслеживать ключевые метрики в реальном времени и прогнозы по полученным данным метрик.**

### English

The goal is to optimize the interaction between management and analytics for medium businesses, enabling real-time data tracking of key metrics and predictions based on received metric data.

## Задачи

Задачей данного проекта является разработка работающего прототипа устройства (MVP) для проверки ключевых гипотез, в связи с чем строгое следование формальным стандартам проектирования на данном этапе не предусмотрено.

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

## Результат

**Пример лога при успешном запуске пайплайна счетчика клиентов:**

```text
[ИНФО] [Pipeline] Инициализация C++ ядра...
[ИНФО] [Inference Engine] Загрузка TensorRT движка: ../../../models/yolo26s.engine
[ИНФО] [Inference Engine] Имя входа: images
[ИНФО] [Inference Engine] Имя выхода: output0
[ИНФО] [Inference Engine] Запуск десериализации движка...
[ИНФО] [Inference Engine] Размер файла движка: 219626764 байт.


```

## Описание (Description)

Для обнаружения объектов применяется модель [YOLO26s](https://huggingface.co/Ultralytics/YOLO26).



Для проверки работоспособности и корректности отправки данных на удаленный сервер выполните запуск скрипта [test.py](scripts/test.py).

Устройство с периодичностью 5 минут в формате JSON производит отправку агрегированных данных на удаленный сервер.

**Формат данных передаваемых на удаленный сервер:**

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

**Ключевые параметры:**

* **interval_start:** Дата и время начала периода агрегации данных (ISO 8601).
* **interval_end:** Дата и время окончания периода агрегации данных (ISO 8601).
* **data:** Блок с метриками за указанный интервал.
* **diagnostic:** Информация о текущем состоянии устройства.

После успешного закрытия чека на кассе данные в формате JSON автоматически передаются на устройство. Устройство производит запись полученных данных в конец во временный лог-файл с расширением `.log`.

**Формат данных записываемых при закрытии чека:**

```json
{"timestamp": "2026-10-21T09::00.00Z", "receipt_id": 0, "total_revenue": 0.0}
```
**Ключевые параметры:**

* **receipt_id:** Идентификатор чека (ID).
* **total_revenue:** Общая выручка.

Для проверки работоспособности сервера и корректности записи чеков выполните POST-запрос с помощью утилиты curl.

**Шаблон запроса:**

```bash
curl -X POST "http://X.X.X.X:X/receipts" \
    -H "Content-type: application/json" \
    -d '{"receipt_id": "X", "total_revenue": X}'
```

С периодом в 1 минуту счетчик клиентов производит запись полученных данных во временный лог-файл, также имеющий расширение `.log`.

**Формат данных записываемых счетчиком клиентов:**

```json
{"timestamp": "2026-10-22T09:16:39.00.00Z", "visitors_in": 0, "visitors_out": 0}
```
**Ключевые параметры:**

* **visitors_in:** Количество клиентов вошедших в заведение.
* **visitors_out:** Количество клиентов вышедших из заведения.

> [!WARNING]
> Все `.log` файлы записываются во временный каталог Linux `/tmp`.

## Нерешенные проблемы
