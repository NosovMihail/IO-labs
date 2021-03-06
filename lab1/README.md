# Лабораторная работа 1

**Название:** "Разработка драйверов символьных устройств"

**Цель работы:** "Получить знания и навыки разработки драйверов символьных устройств для операционной системы Linux

## Описание функциональности драйвера

При записи текста в файл символьного устройства должен осуществляться подсчет введенных символов. 
Последовательность полученных результатов (количество символов) с момента загрузки модуля ядра должна выводиться при чтении созданного файла /proc/varN в консоль пользователя.
При чтении из файла символьного устройства в кольцевой буфер ядра должен осуществляться вывод тех же данных, которые выводятся при чтении файла /proc/varN.

## Инструкция по сборке

```shell
make all
```

## Инструкция пользователя

Загружаем драйвер
```shell
sudo insmod char_dev.ko
```

Выгружаем драйвер
```shell
sudo rmmod char_dev
```


## Примеры использования
Загружаем строку
```shell
echo "2+3" >> /dev/var1
```

Читаем
```shell
cat /dev/var1
>> 5
```

Загружаем ещё строку
```shell
echo "5*7" >> /dev/var1
```


```shell
cat /proc/var1
>> 5
35
```

## Вывод
Можно было сделать лучше и использовать библиотеки для парсинга чисел из строки, но сама задача парсинга была не сложной и реализация "в лоб" была быстрее.

