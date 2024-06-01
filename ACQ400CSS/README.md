# ACQ400CSS
CSS opi project for ACQ400 series digitizers

## CSS: Control System Studio : 
### GUI environment for EPICS
http://controlsystemstudio.org/

#### Ready to run images from here: 

* https://www.d-tacq.com/tmp/cs-studio-4.5.9-win32.win32.x86_64.zip
```

sha1sum cs-studio-4.5.9-win32.win32.x86_64.zip
dbf8fd112e7650a9c35c3a3422deac87968759dd  cs-studio-4.5.9-win32.win32.x86_64.zip

```
* https://www.d-tacq.com/tmp/cs-studio-4.5.9-linux.gtk.x86_64.tar.gz
```
sha1sum cs-studio-4.5.9-linux.gtk.x86_64.tar.gz
1ee699a9e2005f1acb839fe4a433e751d8e5391b  cs-studio-4.5.9-linux.gtk.x86_64.tar.gz

```
* http://download.controlsystemstudio.org/release/4.5/cs-studio-4.5.9-linux.gtk.x86_64.tar.gz
* http://download.controlsystemstudio.org/release/4.5/cs-studio-4.5.9-macosx.cocoa.x86_64.zip
* http://download.controlsystemstudio.org/release/4.5/cs-studio-4.5.9-win32.win32.x86_64.zip

##### version 4.5.9 uses Java 1.8 ("Java8"). 

###### for Windows users: java.com

###### On Ubuntu: 

```
sudo apt-get install openjdk-8-jdk 

# If you want to maintain a newer version of Java, cs-studio/eclipse looks for a local java version first

# assuming cs-studio is unpacked at ~/cs-studio..
ln -s /usr/lib/jvm/java-8-openjdk-amd64/jre ~/cs-studio/
# use alternatives to select global jre version
https://askubuntu.com/questions/740757/switch-between-multiple-java-versions
```



## INSTALL/Configure

### Use Git!

Yes, github will let you download a zip file, but you'll find staying up to date much easier by using git tools to maintain a clone of our repo
On Windows, we strongly recommend https://gitforwindows.org/     And it gives you a bash shell, this is a bonus!

### cs-studio is based on eclipse

#### Concept of Workspace: 

Workspace holds all temp data. **RECOMMEND** set up one workspace per UUT. The workspace will store your configuration (UUT, graphic layout etc) between sessions.

#### Concept of Project: 
The Project is the "code", ACQ400CSS is your project. 
The Workspace references the project.
ONE project is re-used readonly by many workspaces.
**DO NOT** locate the Project in the file tree under the Workspace!

**Recommended File Layout**
1. Store cs-studio in ./cs-studio
2. Store the OPI project in ./PROJECTS/ACQ400CSS
<pre>
mkdir PROJECTS; cd PROJECTS; git clone https://github.com/D-TACQ/ACQ400CSS.git
</pre>
3. Create one workspace per UUT, example: ACQ1001_123: mkdir CSSWS/ACQ1001_123
4. Add the directory PROJECTS/ACQ400CSS/ACQ400 as project "ACQ400" to the workspace. For screen shots please see quickstart below.
5. Set some preferences in your workspace.
<pre>
EDIT | Preferences | CSS Applications | Display | BOY | OPI Runtime
    set name UUT value the name of the uut eg acq1001_123
    ** NOT the ip address! **
EDIT | Preferences | CSS Core | Data Sources | Channel Access :
    set Max Array Size to 500000
</pre>
![cs-studio-environment-settings](cs-studio-environment-settings.png)

# Apologia
 - Sorry, there are way too many steps!. The good news is we used an excellent open source product. The bad news is we have failed to set the best defaults. But, once it's set up, the workspace keeps your configuration faithfully.
 - On starting a new workspace, cs-studio will die unexpectedly, please re-run it when this happens.

### Quickstart Guide: Basic principle applies for all Units Under Test UUT
* Example is for UUT acq1001_190, please substitute the name of your own UUT.

 1. In a well configured system (dhcp, working broadcast route), cs-studio will locate the UUT on the network automatically.
 
 2. Failing that, you'll need to know the default IP address of the UUT
  * UUT NAME, MAC address and default IP-address are printed on the delivery sheet and on faceplate on rear of the box.

# FAQ
 * I have many UUT's, it's not practical to have one workspace per UUT.
  * ANS: oh yes it is. Please create one workspace to use as a template, then clone it many times using our handy [replicator](scripts/make_new_workspace.sh)


#### [Quickstart](https://github.com/D-TACQ/ACQ400CSS/blob/master/acq1001_acq430_quickstart.pdf)
#### [TransientQuickstart](https://github.com/D-TACQ/ACQ400CSS/releases/download/R20210414/acq2106-acq424-transient_capture_quickstart.pdf)
#### [View8 View multiple UUTS](https://github.com/D-TACQ/ACQ400CSS/releases/download/R211119/view8-setup.pdf)


