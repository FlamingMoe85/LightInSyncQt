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
        if(ratio > 0.0) ratio -= fadeInc;
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

    void Load(float _target, qint64 _fadeIn)
    {
       fadeInc = 1.0/(float)_fadeIn;
       target = _target;
       oldSliderVal = (float)mySlider->value();
       ratio = 1.0;
    }

    void SetFadeIn(qint64 _fadeIn)
    {
        fadeInc = 1.0/(float)_fadeIn;
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

        if(caller == 200)
        {

            mySlider->setValue((int)((oldSliderVal*ratio)
                               +
                              ((int)(target*(1.0-ratio)))));
            float retVal = (float)mySlider->value() / mySlider->maximum();
            //qDebug() << "oldSliderVal: " << oldSliderVal << "  ratio: " << ratio << "  target: " << target << "  retVal: " <<  retVal;
            return retVal;
        }
        else
        {
        mySlider->setValue((int)((oldSliderVal*ratio)
                                 +
                                ((int)(target*(1.0-ratio)))));
            }
        return (float)mySlider->value() / mySlider->maximum();
    }

    float GetAbsExtValue(float _targetRel, int caller)
    {
        if(ratio == 0.0) return _targetRel;

        if(caller == 200)
        {

            mySlider->setValue((int)((oldSliderVal*ratio)
                               +
                              ((int)(target*(1.0-ratio)))));
            float retVal = (float)mySlider->value() / mySlider->maximum();
            //qDebug() << "oldSliderVal: " << oldSliderVal << "  ratio: " << ratio << "  target: " << target << "  retVal: " <<  retVal;
            return retVal;
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
    float fadeInc;
};

#endif // CHANGESLIDERATRATE_H
