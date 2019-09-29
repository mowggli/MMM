# PROTO

In 2018 during Ivy Sessions a prototype of an interface for neuroscientists was coded. The basis markup language was not specified with XSD.
This new version named 'proto' is similar but the basis is changed to MMMML.

---

## main.c + choice.c

Asks to choose between:
- 'B' to use basic source interface
- 'S' if you want to create some files based on a predefined question unit.
- 'P' if you want to play the part of a neuroscientist registering a new question unit.
- 'M' if you want to two pre-registered studies.

fundtions invoked depending on choice are defined in choice.c
