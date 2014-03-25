Routeplanner
============
This is a route planner, whick gives the shortest route between two points on an 5 by 5 grid.
## Synopsis

This routeplanner find a route between a starting checkpoint on the edge of a **5 by 5 grid** and one or more endpoints. It can go by the checkpoints in a given order or via the shortest route that visits them all.
<img src='docs/competition_field.pdf' width='80%'>

## Code Example
```
➜  Routeplanner Toine git:(master) ./bin/routeplanner.out 1 5

    (0, 4)   9:(1, 4)   8:(2, 4)   7:(3, 4)     (4, 4)
 10:(0, 3)     (1, 3)     (2, 3)     (3, 3)   6:(4, 3)
 11:(0, 2)     (1, 2)     (2, 2)     (3, 2)   5:(4, 2)
 12:(0, 1)     (1, 1)     (2, 1)     (3, 1)   4:(4, 1)
    (0, 0)   1:(1, 0)   2:(2, 0)   3:(3, 0)     (4, 0)

Start	1: (1, 0)
End		5: (4, 2)

(1, 0) ▷ (2, 0) ▷ (3, 0) ▷ (4, 0) ▷ (4, 1) ▷ (4, 2)
```

## Motivation

This program wa written to guide an autonomous robot, as part of EPO2, a project of Electrical Engineering at the University of Technology Delft.

## Installation
```
cd <download_folder>/Routeplanner\ Toine/
make
```
And then `./bin/routeplanner.out 1 5`, this will give the shortest route from checkpoint 1 to checkpoint 5.

## Contributors

Projectgroep B2:

*	Daniël Brouwer
*	Toine Hartman
*	Dennis de Jong
*	Sam de Jong
*	George Koolman
*	Wesley Umans
*	Luc van Wietmarschen

The method to find the shortest route is based on [Lee's algorithm](http://en.wikipedia.org/wiki/Lee_algorithm "Lee algorithm, Wikipedia").