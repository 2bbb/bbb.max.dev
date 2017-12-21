#/bin/bash

DATE=`date "+%Y%m%d_%H%M%S"`
ROOT_DIR=$(cd $(dirname $0); cd ..;pwd)
PROJECT_DIR=$ROOT_DIR/projects
PACKAGE_DIR=$ROOT_DIR/package_$DATE

echo "start packaging to '$PACKAGE_DIR'..."
if [ ! -d $PACKAGE_DIR ]; then
	echo "$PACKAGE_DIR isn't exist"
	echo "create $PACKAGE_DIR"
	mkdir $PACKAGE_DIR
fi

LIST=`ls $PROJECT_DIR`
for PROJ in $LIST; do
	if [ ${PROJ} = "template" ]; then
		continue
	fi
	echo "copy mxo & maxhelp from $PROJ/build"
	cp $PROJECT_DIR/$PROJ/build/$PROJ.maxhelp $PACKAGE_DIR
	cp -r $PROJECT_DIR/$PROJ/build/$PROJ.mxo $PACKAGE_DIR/$PROJ.mxo
done