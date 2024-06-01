#!/bin/bash

# This is a script that copies an existing CS Studio workspace and replaces all of
# the instances of the original hostname with the new ones.

# new_uut is the uut workspace which will be created from the template.

# template_uut is the uut workspace which will be used to create the new workspace.
# Users can change this to whatever template they like. A UUT workspace "default_uut"
# is the default. 
template_uut=default_uut

# Set workspace location to the CSS Community edition default.
# Change this if you chose a custom location or are using an older
# version of CSS.
workspace_location=/home/$USER/CSS-Workspaces/ 

[ -e $workspace_location ] || workspace_location=/home/$USER/CSS-WORKSPACES/ 
[ -e $workspace_location ] || workspace_location=/home/$USER/CSSWS/;          
[ -e $workspace_location ] || \
	(echo "ERROR: workspace_location not found, please edit script to suit your setup"; exit 1)


usage() {
	cat - <<EOF

Usage examples:
./make_new_workspace.sh -n acq2106_999 -t default_uut
The above is the same as:
./make_new_workspace.sh --new_uut=acq2106_999 --template_uut=default_uut

The new UUT is the UUT which is to be created and the template UUT
is the workspace to be used to create the new one.

Make in batch
./make_new_workspace.sh --template_uut=default_uut -- newuut1 [newuut2 ...]

Make in batch from list of uuts
./make_new_workspace.sh --template_uut=default_uut -- - < list-of-uuts.txt

or with less typing:
./make_new_workspace.sh -t default_uut -- newuut1 [newuut2 ...]
./make_new_workspace.sh -t default_uut -- - < list-of-uuts.txt

EOF
    exit 1
}


# Doing some argument processing here.
while [ "$#" -gt 0 ]; do
case "$1" in
    -n) new_uut="$2"; shift 2;;
    -t) template_uut="$2"; shift 2;;
    -l) workspace_location=$2; shift 2;;
    -h) usage; shift 2;;

    --new_uut=*) new_uut="${1#*=}"; shift 1;;
    --template_uut=*) template_uut="${1#*=}"; shift 1;;
    --workspace_location=*) workspace_location="${1#*=}"; shift 1;;
    --help=*) usage; shift 1;;

    --) echo "batch mode"; break;;
    -*) echo "unknown option: $1" >&2; exit 1;;
    *)  echo "unwanted arg: $1"   >&2; exit 1;;
esac
done

# handle batch modes ..
#
if [ "x$1" = "x--" ]; then
	shift
	if [ "x$1" = "x-" ]; then
		while read newuut; do
			#echo $0 --template_uut=$template_uut --new_uut=$newuut
			$0 --template_uut=$template_uut --new_uut=$newuut
		done
	else
		for newuut in $*; do
			#echo $0 --template_uut=$template_uut --new_uut=$newuut
			$0 --template_uut=$template_uut --new_uut=$newuut
			shift
		done
	fi
	exit 0
fi

# individual processing ..
# Determine if the user has input an argument UUT.
if [ "x$new_uut" = "x" ]; then
    usage
fi


# Move into the CSS-Workspaces directory.
cd $workspace_location


# Check the UUT workspace directory does not already exist.
if [ -d "$new_uut" ]; then
    echo ""
    echo "This workspace already exists. If you want to create it again then remove it first."
    echo ""
    exit 1
fi

echo ""
echo "Create ${workspace_location}${new_uut} from template ${workspace_location}${template_uut}"

# Recursively copy a default UUT workspace to the new one.
cp -r $template_uut/ $new_uut/

# Move into the new directory.
cd $new_uut

# Do a recursive search and replace for the new hostname.
find . -type f -exec sed -i 's/'$template_uut'/'$new_uut'/g' {} +

exit 0
