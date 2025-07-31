#ifndef CHANGESLIDERATRATE_H
#define CHANGESLIDERATRATE_H

#include <QSlider>
#include <QDebug>

static int instCntr = 0;

class ChangeSliderAtRate
{
public:
    ChangeSliderAtRate()
    {
        ratio = 0.0;
        inst = instCntr;
        instCntr++;
    }


    void Ping()
    {
        if(ratio > 0.0) ratio -= 0.005;
        if(ratio < 0.0) ratio = 0;
      /*  if(rate == 0.0) return;
        step += rate;
        if((step >= 1.0) || (step <= -1.0))
        {
            int intStep = (int)step;
            mySlider->setValue(mySlider->value()+intStep);
            step -= intStep;
            if(mySlider->value() == target)rate = 0;
        }*/
    }

    void Load(float _target)
    {
       target = _target;
       oldSliderVal = (float)mySlider->value();
       ratio = 1.0;
    }

    float GetRelValue(float _targetRel)
    {
        if(ratio == 0.0)
        {
            mySlider->setValue((int)((float)mySlider->maximum()*_targetRel));
            return _targetRel;
        }
        mySlider->setValue((int)((oldSliderVal*ratio)
                           +
                          ((float)mySlider->maximum()*_targetRel*(1.0-ratio))));


        return (float)mySlider->value() / (float)mySlider->maximum();
    }

    float GetRelExtValue(float _targetRel, int caller)
    {
        if(ratio == 0.0) return _targetRel;

        if(caller == 7)
        {
            mySlider->setValue((int)((oldSliderVal*ratio)
                               +
                              ((int)(target*(1.0-ratio)))));
        }
        else
        {
        mySlider->setValue((int)((oldSliderVal*ratio)
                                 +
                                ((int)(target*(1.0-ratio)))));
            }
        return (float)mySlider->value();
    }

    QSlider* mySlider;

private:
    float ratio;
    float oldSliderVal;
    float target;
    int inst;
};

#endif // CHANGESLIDERATRATE_H
