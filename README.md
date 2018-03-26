## Домашнее задание 03

[![Build Status](https://travis-ci.org/ithamsteri/homework_03.svg?branch=master)](https://travis-ci.org/ithamsteri/homework_03)

**Цель:** изучить возможности аллокаторов из STL библиотеки

**Необходимо:**
* Написать свой аллокатор, который сможет работать с любым контейнером из STL;
* Написать свой простой контейнер и воспользоваться аллокатором.

**Дополнительно:**
* Добавить диннамическое расширение памяти в аллокаторе.

### Установка пакета

Для Ubuntu Trusty нужно сделать следующее:

```shell
echo "deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu trusty main" | sudo tee -a /etc/apt/sources.list
echo "deb http://hamsterisoft.bintray.com/apt-repo trusty main" | sudo tee -a /etc/apt/sources.list
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1E9377A2BA9EF27F
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 379CE192D401AB61
sudo apt-get update && sudo apt-get install libstdc++7 allocator
```

### Генерация сборки

Запустить в корневом каталоге проекта

```shell
cmake -H. -BBuild
```

### Сборка проекта

Запустить в каталоге Build

```shell
cmake --build .
```
