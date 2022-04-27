# Lab5: Docker and Anaconda
(Work in Group of 2)
In this lab, you will be learning about Docker and Anaconda. Two different types of
virtualization software
Write a simple Docker Build script to install Anaconda and Jupyter notebook in it.
Hints:

1) Assume you have the anaconda installation script already downloaded in your machine
(You should already have it from the lab class)
2) Download a simple Ubuntu docker image (docker pull
<image_name_from_dockerHub>)
3) Write the Docker script at the same place where the anaconda installation script is
downloaded
4) While writing the Docker build script, you need to do the following inside the script:
a. In the build script you need to write the command to copy the anaconda
installation script inside Docker
b. Change its permission using chmod 777
<installation_script_name>
c. Run that script to install Anaconda inside Docker (You need to install Anaconda
in silent mode to avoid interrupting question regarding license accept, location
preference, etc).
d. Then create a Conda environment (conda create -n <envName>) inside
docker
e. Then, activate the conda environment inside docker (conda activate
<env_name>)
f. Finally install jupyter notebook (conda install -c anaconda
jupyter)
** There will be partial score for each of the steps of implementation. 
