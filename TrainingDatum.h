#ifndef TRAININGDATA_H
#define TRAININGDATA_H

template <typename T>
class TrainingDatum
{
public:
    TrainingDatum()
    {
        _observation = T();
        _classification = -1;
    }

    TrainingDatum(const TrainingDatum<T>& other)
    {
        copy(other);
    }

    TrainingDatum(const T& observation, int classification)
    {
        _observation = observation;
        _classification = classification;
    }


    TrainingDatum<T>& operator=(const TrainingDatum<T>& other)
    {
        copy(other);
        return *this;
    }

    void setObservation(const T& observation)
    {
        _observation = observation();
    }

    const T& observation() const
    {
        return _observation;
    }

    void setClassification(int classification)
    {
        _classification = classification;
    }

    int classification() const
    {
        return _classification;
    }

private:
    void copy(const TrainingDatum<T>& other)
    {
        _observation = other._observation;
        _classification = other._classification;
    }

    T _observation;
    int _classification;
};

#endif // TRAININGDATA_H
