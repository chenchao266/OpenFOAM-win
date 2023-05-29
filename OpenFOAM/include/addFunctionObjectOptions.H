#include "addDictOption.H"

 namespace Foam{
argList::addOption
(
    "field",
    "name",
    "Specify the name of the field to be processed, e.g. U"
);
argList::addOption
(
    "fields",
    "list",
    "Specify a list of fields to be processed, e.g. '(U T p)'"
);
argList::addOption
(
    "func",
    "name",
    "Specify the name of the functionObject to execute, e.g. Q"
);
argList::addOption
(
    "funcs",
    "list",
    "Specify the names of the functionObjects to execute, e.g. '(Q div(U))'"
);
argList::addBoolOption
(
    "list",
    "List the available configured functionObjects"
);

 } // End namespace Foam
