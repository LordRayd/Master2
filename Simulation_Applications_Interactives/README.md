# Simulation et Applications Interactives
## Enseignant
Caroline Larboulette
## Objectifs
Ce cours est un cours avancé d’informatique graphique qui se focalise sur l’animation et la simulation de phénomènes 3D en temps-réel.  
Ces simulations sont couplées à des techniques de capture du mouvement (UE Mouvement et Interaction Gestuelle) pour créer des applications interactives contrôlées par l’humain. 
Nous verrons aussi comment valider / améliorer les couplages et les applications réalisées à l’aide d’études perceptuelles. 

* Techniques de déformation géométriques
* Techniques de déformation basées physique (simulation): masses-ressorts, particules, fluides
* Déformation de systèmes complexes et multi-couches: personnages (facial, cheveux, muscles), végétation
* Mise en place d’une application interactive: couplage entre les descripteurs de mouvement et les paramètres d’une simulation
* Informatique graphique basée perception: études perceptuelles

La partie pratique de ce cours est un projet sur lequel les étudiants travaillent en petits groupes pendant toute la durée du semestre et expérimentent avec des capteurs et des dispositifs de rendu innovants tels que le Leap Motion, le Myo Armband, l’Emotiv EPOC ou encore l’HTC Vive.

## Partiel Moodle

Let’s say you have a non-planar polygon made of 4 vertices (i.e. a quad), namely p1, p2, p3 and p4. You want to extrude this face in the direction of its normal  for a length  = 3 units. How do you compute de positions of the new points, p5, p6, p7 and p8 ?

You want to do some 3D morphing between two different models: the head of two characters for example. Propose a possible way to do that (including how you represent your surfaces, how you deform them, and description of any intermediate representation you could use … as well as any necessary constraint you might have).
"We would like to create a virtual zoo where we could see lions without barriers. To add realism, wind is modeled in the scene and the fur of the lion reacts to the wind.
How are the animals modeled and animated ?"

We would like to create a virtual zoo, a real-time virtual reality application. One aim of the application is to be able to select a distant animal to watch it closely and get detailed information. In particular, we would like to be able to first select the animal, then navigate inside it to see the various organs such a the heart, brain, etc...

Describe how the 3D interaction is performed (technique and algorithm) for both, the selection and navigation inside the animal. You may use any additional device to the head mounted display you find necessary.

You want to add muscles on a 3D character animated thanks to a skeleton. You are especially interested in isotoniccontraction.

You want to add short and curly dynamic hair on a 3D character animated thanks to a skeleton,1 propose and describe a model capable of doing that, draw if necessecary 2 what are the advantage drawbakcs of the proposed model


"We would like to create a virtual zoo, a real-time virtual reality application. One aim of the application is to be able to select a distant animal to watch it closely and get detailed information. In particular, we would like to be able to first select the animal, then rotate it, zoom on some parts and switch between two visualisation modes (see the outside or see inside with transparent models of organs).

Describe how the 3D interaction is performed (technique and algorithm) for both, the selection and manipulation (including switching modes). You may use any additional device to the head mounted display you find necessary."
You want to add plants and trees in a 3D simulation of a landscape evolving through time. You want the plants and trees to be highly realistic, but all must be a little bit different from each other. They must also grow over the years, hence the model will evolve with time and climatic conditions.

Mass-Spring Systems (3 points, 20 min)
Imagine you have two particles, P1 with mass m1 = 1 Kg and P2 with mass m2 = 2 Kg. Both are linked by a linear spring of stiffness k = 1 and rest length l = 4 meters (no damping). At init, P1 is at position (0,0,0) and P2 at position (0,0,-4). The node P1 is constrained and moves at constant velocity along the x axis: v = (1, 0, 0). The only external force applying on P2 is gravity on -z axis (no air forces). To compute gravity, you should use g = 9.81.10^-3 m.s^-2 that you may round up to 10^-2 to simplify the computations.
Compute the trajectory of node P2 for 40 seconds of animation. You can choose a timestep dt = 10 seconds and any integration scheme you like (but precise the equations of the one you use). What do you observe ? Why does this happen ? Why not use the real g constant ?

"We would like to create and animate a summer scene consisting of a dancing lemon tree on the beach evolving in real-time. The tree is stylistic (not realistic) and wind is modeled in the scene thanks to a directional vector field that makes the tree sway.
How are the trees modeled and animated ?
(Hint:
 How do you model the trees ? (type of surface, how modeled) ; How do 
you animate the model ? choose and describe a deformation technique that
 could achieve this (we do not ask how it is animated (i.e. FK, mocap, 
etc...) but how the deformation will be computed))."

Let m1 and m2 be two punctual masses, or nodes. They are linked with a spring of rest length l0. What are the forces applying to the nodes ?

"You want to do some 3D morphing between two different models: the
 head of two characters for example. Propose a possible way to do that 
(including how you represent your surfaces, how you deform them, and 
description of any intermediate representation you could use … as well 
as any necessary constraint you might have).
Can the technique you proposed be real-time ? Why ? "

Cloth (1 point, 5 min)You want to create a simple cloth animation. For this, you use a mesh of quads. You then select a vertex of the mesh that you pull. You want the mesh to be deformed so that the other vertices gradually follow the one you have pulled. Propose a geometrically based technique to achieve this (give the details). You may draw if it helps.
"You want to create a simple cloth animation. For this, you use a mesh of triangles.
 You then select a vertex of the mesh that you pull. You want the mesh 
to be deformed so that the other vertices gradually follow the one you 
have pulled. Propose a geometrically based technique to achieve this (give the details). You may draw if it helps. "
Collision Detection (2 points, 10 min) : collider unity (: