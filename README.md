# Design-adaptive-path-for-self-driving-car

Designing an adaptive path for a self-driving car included computation of
shortest path to reach destination and setting up control systems of the car
by incorporating many factors. To achieve this, we have formatted a step
wise process, as listed below:
1. Computing a predefined imaginary reference rectangle, with eighteen
segments inside it, In-front of the car.
2. Computing a new path from current location of the car to the destination.
3. Setting up membership functions for each category using fuzzy logic.
4. Computing the speed of the self driving car depending on the membership
functions and Inference Engine, a field of artificial intelligence.
1 Objective:
Compute pre-defined ”PATH” from the current location of the self-driving
car.
• Right turn: 3 x 3 = 9 segment paths
• Left turn: 3 x 3 = 9 segment paths

u = length of the car. B = Big turn, M = Medium turn, S = Sharp turn.

- B (9*u/9)
- B+M (8*u/9)
- B+S (7*u/9)
- M (6*u/9)
- M+M (5*u/9)
- M+S (4*u/9)
- S (3*u/9)
- S+M (2*u/9)
- S+S (1*u/9)


2 Feature list:

• For any one of the 9+9 categories, we will have to compute the fpwm, duty-cycle of the signal to drive the motor to realize physical turn.

• For any given target position, the newest check point (xi+1, yi+1),
assuming the current point is (xi, yi).

• Assuming the check point is inside the pre-defined reference path, concentrate on point Pi+1 = (xi+1, yi+1).

• The segment path, Fs,pq(x, y) 
[s = right(r) or left(l), p ∈ (s, m, b), q ∈ (s, m, b), (x, y) = coordinates of reference point through which the parabola is passing].

• Compute two segment paths which are closest to the newest check point, call first closest one as minimum Fs,p1q1(x,y) and rank one Fs,p1q1(x,y).

• Find the path from given Pi+1 (xi+1, yi+1) and Fs,p1q1(x,y), Fs,p1q1(x,y).


3 Algorithm:

• Compute the distance form checkpoint Pi+1 (xi+1, yi+1) to the newest path, Min dist [Pi+1 (xi+1, yi+1) - Fs,p1q1(x,y)] ........ Fs,pqmin , then Rank1 dist [Pi+1 (xi+1, yi+1) - Fs,p2q2(x,y)] ....... Fs,pq rank1.

• Distance defined as follows for each xi, the distance is, (yi,pi - yi,fs,pq )^2 = (yi,pi - yi,fr ,pq )^2. Generate a new path, Fs,new(x, y), for each xi, from Fs,new (x, y), where xi = xi, xi + 1, xi + 2, . . . , xi+1.

• Find yi,fs,pqmin, Example : Given fs,pqmin (x,y) = 0, x = xi, find yi which is yi,fs,pqmin.

• Find yi,fs,pqrank1, Example: Given fs,pqrank1(x,y) = 0, x = xi, find yi which is yi,fs,pqrank1.

• Find the new path from previous equation, as follows,
yi = 1/2 * (yi,fs,pqmin + yi,fs,pqrank1)

• Based on fuzzy logic membership function, define membership for each
segment ”sharp”, ”medium”, ”big”, etc.

• Therefore, the heuristics can be incorporated, Example: if the turn is S, them the speed is sp1, if the turn is M, them the speed is sp2 , where sp2 is greater than sp1.
