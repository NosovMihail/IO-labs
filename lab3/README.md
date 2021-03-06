# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы Linux.

## Описание функциональности драйвера

Созданный сетевой интерфейс перехватывает пакеты протокола IP, отправленные по определённому адресу. Информация о перехваченных пакетах выводится в файл в /proc и в кольцевой буфер ядра.


## Инструкция по сборке

`$ make`

## Инструкция пользователя

`# insmod virt_net_if.ko input_ip="<target ip>"`

## Примеры использования
Отправка сообщения на интерфейс

`ping "224.0.0.251"`

Результат перехвата в кольцевом буфере

![image](https://user-images.githubusercontent.com/32123595/173808938-f1dfcef8-4bc0-492a-a9bb-c91bee0f5071.png)

Результат перехвата в файле в /proc

![image](https://user-images.githubusercontent.com/32123595/173808826-94933e7b-ca67-4e37-9a0d-de0c830420be.png)
