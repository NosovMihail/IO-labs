# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** получить знания и навыки разработки драйверов блочных
устройств для операционной системы Linux.

## Описание функциональности драйвера

Драйвер создаёт виртуальный жесткий диск содержащий два первичных и один расширенный разделы с размерами
10Мбайт, 20Мбайт и 20Мбайт соответственно. Расширенный
раздел разделен на два логических с размерами
по 10Мбайт каждый.

## Инструкция по сборке

Для сборки вызвать make в директории с исходными файлами

## Инструкция пользователя

Для установки модуля использовать insmod main.ko, для удаления - rmmod main с правами суперпользователя.
Для форматирования разделов можно использовать mkfs.vfat /dev/mydisk1

## Примеры использования

sudo mkfs.vfat /dev/mydisk1
sudo mount -t vfat mydisk1 /mnt/1


## Тесты скорости копирования

#### С диска в память
```
sudo rsync -av --progress --stats test.txt  /mnt/1/
sending incremental file list
test.txt
      5,242,880 100%   62.49MB/s    0:00:00 (xfr#1, to-chk=0/1)

Number of files: 1 (reg: 1)
Number of created files: 1 (reg: 1)
Number of deleted files: 0
Number of regular files transferred: 1
Total file size: 5,242,880 bytes
Total transferred file size: 5,242,880 bytes
Literal data: 5,242,880 bytes
Matched data: 0 bytes
File list size: 0
File list generation time: 0.001 seconds
File list transfer time: 0.000 seconds
Total bytes sent: 5,244,266
Total bytes received: 114

sent 5,244,266 bytes  received 114 bytes  10,488,760.00 bytes/sec
total size is 5,242,880  speedup is 1.00

#### Из памяти в память
```
sudo rsync -av --progress --stats test.txt  /mnt/2/
sending incremental file list
test.txt
      5,242,880 100%   72.17MB/s    0:00:00 (xfr#1, to-chk=0/1)

Number of files: 1 (reg: 1)
Number of created files: 1 (reg: 1)
Number of deleted files: 0
Number of regular files transferred: 1
Total file size: 5,242,880 bytes
Total transferred file size: 5,242,880 bytes
Literal data: 5,242,880 bytes
Matched data: 0 bytes
File list size: 0
File list generation time: 0.001 seconds
File list transfer time: 0.000 seconds
Total bytes sent: 5,244,240
Total bytes received: 35

sent 5,244,240 bytes  received 35 bytes  10,488,550.00 bytes/sec
total size is 5,242,880  speedup is 1.00
```
