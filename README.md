#### The aim of this project is to create a program capable of imitating Dropbox.

The user should be able to select a folder through a suitable GUI of its own client.

The files of the selected folder are stored inside a server (possibly running on other pc)

The client should communicate to the server all the changes that the user made on the tracked files.

The user should have the possibility to add/remove files from the folder, to select a past version from the server of a file and to download it.

We are going to use c++ to develop the core of the system an later on c# to develop the GUI.

The server will be a multithread process as well as the GUI and the client, we will probably use pipes in order to communicate between processes.

Inside PROGETTO/MALNATI you can find the actual solution; inside the other folders you can find all the stub that the project gonna use
