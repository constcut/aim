#include "userSettings.h"

UserSettings::UserSettings(QObject *parent):QObject(parent)
{
    //here now lay defaults but later save them to base and load from there

    setColor("Accent","#4CAF50"); //Material.Green
    setColor("Primary","#3F51B5"); //Material.Ingido
    setColor("Foreground","#000000"); // #795548or Material.Brown
    setColor("Background","#FFFFFF"); //or Material.Grey#607D8B
    setColor("ListHightlight","#98FB98");
    ///maybe better store as propeties to be able to update asap


    addAimSettings["ListName"] = true;
    addAimSettings["TimeAndDate"] = true;
    addAimSettings["Category"] = true;
    addAimSettings["Repeatable"] = true;
    addAimSettings["Privacy"] = true;
    addAimSettings["AssignTo"] = true;
    addAimSettings["ParentAim"] = true;
    addAimSettings["ChildAims"] = true;
    addAimSettings["Progress"] = true;


    viewAimSettings["ListName"] = true;
    viewAimSettings["TimeAndDate"] = true; //false;
    viewAimSettings["Category"] = true;
    viewAimSettings["Repeatable"] = true; //false;
    viewAimSettings["Privacy"] = true; //false;
    viewAimSettings["AssignTo"] = true; //false;
    viewAimSettings["ParentAim"] = true; //false;
    viewAimSettings["ChildAims"] = true; //false;
    viewAimSettings["Progress"] = true; //false;
}


QList<bool> UserSettings::getAddAimSettings()
{
    QList<bool> response;

    response.append(viewAimSettings["ListName"]);
    response.append(viewAimSettings["TimeAndDate"]);
    response.append(viewAimSettings["Category"]);
    response.append(viewAimSettings["Repeatable"]);
    response.append(viewAimSettings["Privacy"]);
    response.append(viewAimSettings["AssignTo"]);
    response.append(viewAimSettings["ParentAim"]);
    response.append(viewAimSettings["ChildAims"]);
    response.append(viewAimSettings["Progress"]);

    return response;
}

void UserSettings::setAddAimSettings(QList<bool> newSettings)
{
    addAimSettings["ListName"] = newSettings[0];
    addAimSettings["TimeAndDate"] = newSettings[1];
    addAimSettings["Category"] = newSettings[2];
    addAimSettings["Repeatable"] = newSettings[3];
    addAimSettings["Privacy"] = newSettings[4];
    addAimSettings["AssignTo"] = newSettings[5];
    addAimSettings["ParentAim"] = newSettings[6];
    addAimSettings["ChildAims"] = newSettings[7];
    addAimSettings["Progress"] = newSettings[8];
}

///helpable function to set up ui in roper good way

QList<bool> UserSettings::getViewAimSettings()
{
    QList<bool> response;

    response.append(viewAimSettings["ListName"]);
    response.append(viewAimSettings["TimeAndDate"]);
    response.append(viewAimSettings["Category"]);
    response.append(viewAimSettings["Repeatable"]);
    response.append(viewAimSettings["Privacy"]);
    response.append(viewAimSettings["AssignTo"]);
    response.append(viewAimSettings["ParentAim"]);
    response.append(viewAimSettings["ChildAims"]);
    response.append(viewAimSettings["Progress"]);

    return response;
}

void UserSettings::setViewAimSettings(QList<bool> newSettings)
{
    viewAimSettings["ListName"] = newSettings[0];
    viewAimSettings["TimeAndDate"] = newSettings[1];
    viewAimSettings["Category"] = newSettings[2];
    viewAimSettings["Repeatable"] = newSettings[3];
    viewAimSettings["Privacy"] = newSettings[4];
    viewAimSettings["AssignTo"] = newSettings[5];
    viewAimSettings["ParentAim"] = newSettings[6];
    viewAimSettings["ChildAims"] = newSettings[7];
    viewAimSettings["Progress"] = newSettings[8];
}

//colors functions
void UserSettings::setStyle(QString styleName, QString themeName)
{

}

bool UserSettings::setColor(QString colorType, QString colorValue)
{
    bool colorExists = colors.find(colorType) != colors.end();
    colors[colorType]=colorValue;
    return colorExists;
}

QString UserSettings::getColor(QString colorType)
{
    bool colorExists = colors.find(colorType) != colors.end();
    if (colorExists)
        return colors[colorType];
    return "";
}
