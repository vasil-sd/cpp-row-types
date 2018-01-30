# Row types in C++ templates

This template library may be helpful for someone, who looks for reflection in C++, row types and some type system in templates.

For good example of reflexivity, see file example.h GenerateId template.

## Preliminary roadmap:

### From scientific point of view:
1. Unification
2. Pattern-matching based on 1
3. Lambda-calculus with types and beta-reduction via cbv/cbn
4. Previous point starts a road to inductive constructions, inductive defined types and so on.
5. Full support of dependent types.

### From engineering point of view:
1. DONE Rework and improve type coercion part.
2. Separate types, their representation and their storage.
   This opens whole new world of using data types in code, where code can be abstracted even from representation of data types.
   Think of code, which does not care if numbers represented as 32bit uints, or as strings of decimal digits, or as numbers in residue number system.
   And its storage may be in memory, special scratchpad memory, hdd etc... But main algorithm remains literally the same in all these cases.
   Of course, there are numerous subtle things that should be taken into consideration, but various constraints may be moved to low-level datatype representation code (some overflow/underflow checks and so on).
3. Previous point allows us to control exact types representation in storage. Thus C++ gets ability to control records layout as in Ada language. This should be wellcomed from system programmers.
