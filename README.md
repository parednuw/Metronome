# Metronome

This is a metronome I built upon a tutorial from The Audio Programmer: https://www.youtube.com/watch?v=z3XDvYmTarE&t

I changed the metronome-program a bit, added a way to change the sound of the click and added a Countdown-Timer to focus on each exercise for a specific amount of time in minutes

The file-reading for the click-sound is not very effective right now. Everytime you change the sound, the filepath is read and the wanted sound is being sought. Maybe creating unique_ptr to the files at prepareToPlay is more efficient.
Also the path to the Audio-folder is absolute and needs to be set in Metronome.cpp, line 22.
