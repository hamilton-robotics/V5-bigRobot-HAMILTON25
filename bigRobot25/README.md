**DECEMBTER 2 UPDATE:** 

__Done:__  
Draft of all big robot code 

__To Do:__  
Tune PID\
Tune color sensors to mount\
Get uploading working on literally anyones laptop! 

__Bugs:__  
The largest object in vision sensor is not correct. It always uses red/team if it's seen.\


**NOVEMBER 18 UPDATE:** 

__Done:__  
Got color sensor working including with intake! 

__To Do:__  
Tune PID\
Tune color sensors to mount\
Get uploading working on literally anyones laptop! 

__Bugs:__  
The largest object in vision sensor is not correct. It always uses red/team if it's seen.\

**NOVEMBER 11 UPDATE:** 

__Done:__  
Confirmed intake code works \
Neatened code a bit \
Made progress on understanding color sensor code 

__To Do:__  
Tune PID\
Fix color sensors\
Get uploading working on literally anyones laptop! 

__Bugs:__  
Can get it to sense one color, but can't get it to change behavior when color changes/goes away

**NOVEMBER 15 UPDATE:** 

__Done:__  
Made progress on understanding color sensor code? 

__To Do:__  
Tune PID\
Fix color sensors\
Get uploading working on literally anyones laptop! 

__Bugs:__  
Honestly, I don't know. Something is going wrong, we're maybe getting closer, but its not going great

**NOVEMBER 11 UPDATE:** 

__Done:__  
Confirmed intake code works \
Neatened code a bit \
Made progress on understanding color sensor code 

__To Do:__  
Tune PID\
Fix color sensors\
Get uploading working on literally anyones laptop! 

__Bugs:__  
Can get it to sense one color, but can't get it to change behavior when color changes/goes away

**NOVEMBER 8 UPDATE:** 

__Done:__  
Attempted a new user to upload from working computer (did not work) \
Tested intake motors (code appears bug free but couldn't test bc of uploading) 

__To Do:__  
Tune PID\
Test color sensors\
Get uploading working on literally anyones laptop!
Properly test intake motors 

__Bugs:__  
Unknown because of upload issues

**NOVEMBER 1 UPDATE:** 

__Done:__  
Figured out general vibe of how vision sensor works \
Found color signatures? 

__To Do:__  
Tune PID\
Test color sensors\
Get uploading working on literally anyones laptop!

__Bugs:__  
Code compiles, but is not actually getting color sensor information. We have confirmed not a hardware issue

__Relevant Links:__  
Vision Sensor C++ API: https://pros.cs.purdue.edu/v5/api/cpp/vision.html#constructor-s \
Vision Secsor API: https://pros.cs.purdue.edu/v5/tutorials/topical/vision.html

**OCTOBER 28 UPDATE:**  

__Done:__  
Figured out that Mac has fewer issues with github, pros, and the robot itself \
Got drive working \
Got single motor working \
Found relevant links (see below) 

__To Do:__  
Tune PID\
Test color sensors\
Get uploading working on literally anyones laptop!

__Bugs:__  
None currently

__Relevant Links:__  
Controller Button Names: https://pros.cs.purdue.edu/v5/api/c/misc.html#controller-digital-e-t \
PID Tuning: https://lemlib.readthedocs.io/en/stable/tutorials/4_pid_tuning.html, https://www.reddit.com/r/matlab/comments/vqk5jc/how_to_tune_pid_controller/ \
Color Sensor: https://pros.cs.purdue.edu/v5/api/cpp/optical.html#get-rgb