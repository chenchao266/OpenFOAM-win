// Check -time and -latestTime options

if (args.found("time"))
{

 namespace Foam{
    scalar timeValue = args.get<scalar>("time");

    startTime = Time::findClosestTimeIndex(Times, timeValue);
}

if (args.found("latestTime"))
{
    startTime = Times.size() - 1;
}

 } // End namespace Foam
