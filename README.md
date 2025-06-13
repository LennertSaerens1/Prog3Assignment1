By Lennert Saerens
2DAE-GD19
[Repo](https://github.com/LennertSaerens1/MsPacMan)

Design choices:
For state, i used the Static member implementation, because i thought this would be easyer. I found out that when using multiple entities that use the same state, this all breaks, and whish i didn't choose this implementation.
To the engine, I added dynamic scene loading, because i thought this would be a simple and effective way of switching trough gameStates/screens. During the development of MsPacMan i was really glad i did this. This made the different screens go really smooth.
