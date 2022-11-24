# triangle

My first, non-guided, x64 internal.
I can say with full confidence that I wrote nearly everything in this project (maybe a few math structures and magic numbers borrowed from a few indians).
There are a few things that I could fix, that I geniunely dont care enough to just because it's a personal project.

A few things to note:
- Seems like swap buffers (atleast during testing) crashed on unhook. It should work fine now but just be aware that your game may crash on un-injection. (very low chance)
- Sometimes the raycast will not hook properly, if this happens just re-inject and silent aim will work.
- Pointers become really gay on release mode, I guess don't compile on it (oh well)

+/- to increase radius
Press space while infinite jump is enabled to, well, infinitely jump.
F10 to enable menu.
