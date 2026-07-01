  
# ACQ400CSSP

Set of [Phoebus](https://github.com/ControlSystemStudio/phoebus) OPIs based on [ACQ400CSS](https://github.com/D-TACQ/ACQ400CSS) for [D-Tacq](https://d-tacq.co.uk/) Digitizers.

![multi_1_opi](https://github.com/sambelltacq/ACQ400CSSP/releases/download/v1.0.0/multi_1_opi.png)
More [examples](https://github.com/sambelltacq/ACQ400CSSP/blob/master/USERGUIDE.md)

## Installation

- Clone repo
    ```bash
    git clone https://github.com/D-TACQ/ACQ400CSSP
    cd ACQ400CSSP
    ```

- Install [Java17+](https://adoptium.net/en-GB/temurin/releases/?os=any&arch=any&version=17)
- Download and extract Phoebus (5.0.2):
	- [Linux](https://github.com/ControlSystemStudio/phoebus/releases/download/v5.0.2/phoebus-5.0.2-linux.tar.gz) 
	- [Windows](https://github.com/ControlSystemStudio/phoebus/releases/download/v5.0.2/phoebus-5.0.2-win.zip)
	- [Mac](https://github.com/ControlSystemStudio/phoebus/releases/download/v5.0.2/phoebus-5.0.2-mac-arm64.tar.gz)

- For ease of installation extract or link the phoebus jar directory into the repo e.g.

    ```bash
    │── ACQ400CSSP
    │   │── ACQ400CSSP_launcher.py
    │   │── workspaces
    │   │── CSSP.conf
    │   │── product-5.0.0
    │   │   │── phoebus-5.0.0.jar
    ```

- You can create a CSSP.conf file to hardcode paths if needed:

    ```bash
    JAVA_BIN=C:\Program Files\Eclipse Adoptium\jre-17.0.12.7-hotspot\bin\java.exe
    PHOEBUS_JAR=C:\Users\sam\PROJECTS\ACQ400CSSP\product-5.0.0\product-5.0.0.jar
    ```

## Usage

#### Python Launcher (recommended)
Single UUT launch:
```bash
./ACQ400CSSP_launcher.py acq2106_999
```
Multi UUT launch:
```bash
./ACQ400CSSP_launcher.py acq2106_997 acq2106_998 acq2106_999
```


Each UUT generates a workspace containing it's memento and settings.ini.
Create a workspace.prefs or a UUT_workspace.prefs to automatically override existing prefs on launch

#### Native scripts (depreciated)

##### Linux script

```bash
./run_phoebus.sh acq2106_999
```

##### Windows script

```bash
run_phoebus.bat acq2106_999
```

## Updating

- Update the repo

```bash
git pull
```

- Erase outdated workspaces

```bash
rm -rf workspaces
```
