#ifndef NEARESTNEIGHBORSMATCHER_H
#define NEARESTNEIGHBORSMATCHER_H

#include "TrainingDatum.h"

#include <QObject>
#include <QMap>

#include <functional>

template <typename T>
class NearestNeighborsMatcher : public QObject
{
public:
    NearestNeighborsMatcher(QObject* parent = nullptr) : QObject(parent), _neighborCount(5)
    {

    }

    void clear()
    {
        _trainingData.clear();
    }

    void setNeighborCount(uint neighborCount)
    {
        _neighborCount = neighborCount;
    }

    uint neighborCount() const
    {
        return _neighborCount;
    }


    void setDistanceFunction(std::function<qreal(T, T)> distanceFunction)
    {
        _distanceFunction = distanceFunction;
    }

    void addTrainingData(const TrainingDatum<T>& trainingData)
    {
        _trainingData.push_back(trainingData);
    }

    int classifyDataPoint(const T& untrainedDataPoint) const
    {
        int result = -1;
        //Note: Boolean check on distance function means that the function is valid
        //We also want to make sure that we have at least one point more than the number of neighbors to check against.
        if (_trainingData.count() > _neighborCount && _distanceFunction)
        {
            QVector<QPair<int, qreal>> distancesWithClass;
            int index = 0;
            for (auto trainingDatum : _trainingData)
            {
                qreal distance = _distanceFunction(untrainedDataPoint, trainingDatum.observation());
                auto pair = qMakePair(trainingDatum.classification(), distance);
                distancesWithClass.push_back(pair);
            }

            std::sort(distancesWithClass.begin(), distancesWithClass.end(), [](const QPair<int, qreal>& firstPair, const QPair<int, qreal>& secondPair)
            {
                return firstPair.second < secondPair.second;
            });

            QMap<int, QPair<int, int>> totals;
            index = 0;
            auto distanceWithClass = distancesWithClass.begin();
            while (distanceWithClass != distancesWithClass.end() && index < _neighborCount)
            {
                if (totals.contains((*distanceWithClass).first))
                {
                    totals[(*distanceWithClass).first].second++;
                }
                else
                {
                    totals[(*distanceWithClass).first] = qMakePair((*distanceWithClass).first, 1);
                }
                ++distanceWithClass;
                ++index;
            }   
            auto totalsAsVector = totals.values();
			std::sort(totalsAsVector.begin(), totalsAsVector.end(), [](const QPair<int, int>& firstPair, const QPair<int, int>& secondPair)
            {
                return firstPair.second > secondPair.second;
            });
            result = totalsAsVector.first().first;
        }

        return result;
    }


private:
    uint _neighborCount;
    std::function<qreal(T, T)> _distanceFunction;
    QVector<TrainingDatum<T>> _trainingData;
};

#endif // NEARESTNEIGHBORSMATCHER_H
