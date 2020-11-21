# tf2-random-disguise
A linux kernel module that makes a device /dev/tf2d that, when read, yields tf2 cfg commands to switch to a random class when playing as spy.


To use:
1. `make all` to generate main.ko kernel module
2. load main.ko using `sudo insmod ./main.ko` 
3. Set up a symlink between the newly created `/dev/tf2d` device and TF2's `tf/cfg` folder. Here the symlink is named randomDisguise.cfg
4. Bind a key to execute `exec randomDisguise` in your `autoexec.cfg` or wherever you keep your custom tf2 settings
5. Profit
