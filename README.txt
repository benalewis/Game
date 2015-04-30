This is an OpenGL 3D Game to serve as my assessment for the Semester 2 Games Development module as an extension of the first. 

Libraries Used: 
 
GLEW (Extensions) http://glew.sourceforge.net/
GLFW (Window & I/O) http://www.glfw.org/
GLM (Maths) http://glm.g-truc.net/0.9.6/index.html
SOIL (Image Loading) http://www.lonesock.net/soil.html

Current Builds:

Mac OS X Yosemite Version 10.10.3 (Xcode Build)
Windows 8.1 (Visual Studio Build)

Marking Scheme:

[X] Working window with blocks on screen 20%:

This was the first hurdle in learning because I had to set up the environment and understand how to
link libraries and dependencies and such. I did so across two platforms (Mac & Windows). After the libraries
where linked the rest is fairly simple from setting the context to creating VBOs and VAOs to loading shaders
and then rendering. 

[X] Floor and walls formed 10%

This part was easier than expected, it just took a fair amount code. Once the block was made it was fairly 
easy to loop through and multiply the same cube and generate it at a different place. 

[X] Textures added 10%

3 Textures were added. I would have loved to separate the textures into their own class so it look less
space on main.cpp but I lack the understanding of c++ as a whole and was terrified of breaking the program

[X] Camera added 15%

Another challenging part but once there was a core understanding of how the camera works in 3D games
it was fairly simple to implement, it also became a staple for movement and collision.

[X] Movement added 10%

Movement was easy once the camera was in place, it was simply a case of readjusting the position once
various keys where entered. 

[?] Maze built 10%

I never ended up building a "maze" because the way I would of done it was to hard code blocks to block
off a certain path which wouldn't show any additional coding ability.

[X] Ice movement 5%

Ice movement was simple and fun, had a lot of fun testing and playing around with the algorithm until it
worked properly. 

[?] Collision detection and objects added to avoid 10%

There is "collision detection" but more so setting boundaries than actually colliding with the objects, which
is why I awarded no points here. This is definitely one of the areas I could build upon. 

[X] End game loss screen added 10%

Scrapped this because the only loss screen I really could implement was to close the window and leave the 
console open with a "You lost" type style, didn't show anymore skill in programming.

Reflection:

Overall I think I met my goals well but I was really dissatisfied with how cluttered my code was. The reason why
I didn't set out my code into the suitable classes and header files was because I was still learning how to
code OpenGL. If i were to start again with the knowledge I had now the code would have been a lot cleaner 
and main.cpp would have been a simple init world, init game loop type file. 

Total: 70-80%