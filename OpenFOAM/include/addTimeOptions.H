
 namespace Foam{
argList::addBoolOption
(
    "constant",
    "include the 'constant/' dir in the times list"
);

argList::addBoolOption
(
    "latestTime",
    "select the latest time"
);

argList::addBoolOption
(
    "noZero",
    "exclude the '0/' dir from the times list"
);

argList::addOption
(
    "time",
    "time",
    "specify a single time value to select"
);

 } // End namespace Foam
