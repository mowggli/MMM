# Ivy sessions initial code Oct 2018

execute in terminal with command ` ./tiptop`  or ` ./tiptop <S|P|M>` followed by 0,1 or 2 markup file names containing each a description of an MMM unit. Default used are `Blip-is-blop-MU.txt` and `Blip-is-blop-MU2.txt`.

---

In **mmm.c**:

*Choice function*

```
(0) Please choose and type:
    'S' if you want to create some files based on a dummy predefined question unit (described in Blip-is-blop-MU.txt).
    'P' if you want to play the part of a neuroscientist registering a new question unit.
    'M' if you want to two pre-registered studies.
```

---

If choice = `S` then `readQUESTION` then `fprintForestQUESTION` (from **forest.c**) a MMM question unit on screen then possibility to `createFiles` (from **addendum.c**).

----

If choice = `M` then same as previous case, but this time with two MMM question units and in the middle a call to `mergeQuestions` (from **interface.c**).

----

If choice = `S` then `neuroscience_mmm` (from **interface.c**)

```
(0a) Do you want to register a study that is...
	    ... not started --> type 'S' (for now same as option 'P'),
	    ...  in process --> type 'P',
	    ...    finished --> type 'F'
```
If choice = `P` then ...
If choice = `F` then ...
If choice = `S` again then ...
