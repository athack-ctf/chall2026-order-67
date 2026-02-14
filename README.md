# Chall - Order 67

> Kill all the child processes and get the flag. Temple of Jedi continuously respawn childs ... PID_LIMIT is the key.

## Challenge Type

- [ ] **OFF**line
- [X] **ON**line

## Design Type

- [ ] **Black**-Box
- [X] **White**-Box

## Designer(s)

- Hugo Kermabon--Bobinnec

## Description

Participants must check the source code and see that they get the flag once all child processes are dead.
There is a limit on the number of PID the program can fork (subtely hidden as "save cost for infra"). Participants must spawn a lot of processes to prevent the server from respawning childrens.


## Category(ies)

- `misc`
