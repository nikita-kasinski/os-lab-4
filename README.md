# Exercise 4 ОS
## Made by Nikita Kasinski
### Overview
The app comprises two types of processes: one Receiver and several Sender processes. Sender send messages to Receiver, which in return reads them. Communication is organized via binary file using FIFO principle.
### Tools
* gcc - compiler
* make - build automation tool
* gtest - library for creating unit tests on c++
* gcov, lcov - tools to test code coverage
### Manual running, building, testing and testing code coverage
To perform any listed task several tools and libraries are required:
* gcc
* make 
* gtest (when testing code and code coverage)
* gcov (when testing code coverage)
* lcov (when testing code coverage)

Prepared targets:
* make run - launching the app
* make build - building the app
* make test - running tests
* make report - testing code coverage
