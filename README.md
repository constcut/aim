# aim

Pet project for learning QML, that grown up into pretty advanced time\task manager.
Base is stored using sqlite.

## Features

* Add new task (aim), it can have: time\date, comments, tree hash tags, priority, parent aim, repeat period
* View or edit single aim, with possibilities: add web links related to aim, navigate through child aims, set current progress, export aim into file (import is also availible)
* Track time of running aim (several aims could be running at the same time)
* Navigate through tree of aims (parent\child)
* Navigate through tree of tags (for example #code.refactoring.cpp and #code.refactoring.qml would be grouped)
* Watch and navigate scheduled aims in calendar
* Watch and navigate tracked time on calendar
* Watch tracked time chart for each day using calendar
* Watch logging in application

Also there was a simple chat using web sockets, but it require additional server so it was removed.


## Description

Main view:

![aim screenshot](info/main.png)

Contains list of aims, with additional information on each aim.
Fields of aims to display could be configured.
There is search field, that let you search aims by name or tag.

Add new:

![aim screenshot](info/add_new.png)

Side pannel that awailible from every other view.
Aim must have name, and could have time\date, comments, tags, priority, tags, assignment (to people, droped with server), parent aim, and repeat period.

