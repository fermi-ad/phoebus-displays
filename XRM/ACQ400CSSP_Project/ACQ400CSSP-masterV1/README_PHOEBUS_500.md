# install Phoebus 5.0.0


## Linux
```
cd PROJECTS/ACQ400CSSP
wget https://github.com/ControlSystemStudio/phoebus/releases/download/v5.0.0/phoebus-5.0.0.tar.gz
tar xvzf phoebus-5.0.0.tar.gz
```

Then make this change to run_phoebus.sh

```
diff --git a/run_phoebus.sh b/run_phoebus.sh
index dc8ebb4..b004f07 100755
--- a/run_phoebus.sh
+++ b/run_phoebus.sh
@@ -17,6 +17,7 @@ fi
 
 # CHANGE ME
 PHOEBUS_JAR="$ROOT_DIR/product-4.7.3/product-4.7.3.jar"
+PHOEBUS_JAR="$ROOT_DIR/product-5.0.0/product-5.0.0.jar"
```


