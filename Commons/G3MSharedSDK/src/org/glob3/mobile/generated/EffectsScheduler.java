package org.glob3.mobile.generated; 
public class EffectsScheduler
{

  private static class EffectRun
  {
    public Effect _effect;
    public EffectTarget _target;

    public boolean _started;

    public EffectRun(Effect effect, EffectTarget target)
    {
       _effect = effect;
       _target = target;
       _started = false;
    }

    public void dispose()
    {
      if (_effect != null)
         _effect.dispose();
    }
  }


  private java.util.ArrayList<EffectRun> _effectsRuns = new java.util.ArrayList<EffectRun>();
  private ITimer _timer;
  private IFactory _factory; // FINAL WORD REMOVE BY CONVERSOR RULE


  private void processFinishedEffects(G3MRenderContext rc, TimeInterval when)
  {
  
    final java.util.Iterator<EffectRun> iterator = _effectsRuns.iterator();
    while (iterator.hasNext()) {
      final EffectRun effectRun = iterator.next();
      if (effectRun._started) {
        final Effect effect = effectRun._effect;
        if (effect.isDone(rc, when)) {
          effect.stop(rc, when);
  
          effectRun.dispose();
          iterator.remove();
        }
      }
    }
  }

  public EffectsScheduler()
  {
     _effectsRuns = new java.util.ArrayList<EffectRun>();
  }

  public final void doOneCyle(G3MRenderContext rc)
  {
    final TimeInterval now = _timer.now();
  
    processFinishedEffects(rc, now);
  
    final java.util.Iterator<EffectRun> iterator = _effectsRuns.iterator();
    while (iterator.hasNext()) {
      final EffectRun effectRun = iterator.next();
      final Effect effect = effectRun._effect;
      if (!effectRun._started) {
        effect.start(rc, now);
        effectRun._started = true;
      }
      effect.doStep(rc, now);
    }
  }

  public final void initialize(G3MContext context)
  {
    _factory = context.getFactory();
    _timer = _factory.createTimer();
  }

  public void dispose()
  {
    _factory.deleteTimer(_timer);

    for (int i = 0; i < _effectsRuns.size(); i++)
    {
      EffectRun effectRun = _effectsRuns.get(i);
      if (effectRun != null)
         effectRun.dispose();
    }
  }

  public final void startEffect(Effect effect, EffectTarget target)
  {
    _effectsRuns.add(new EffectRun(effect, target));
  }

  public final void cancelAllEffects()
  {
    final TimeInterval now = _timer.now();
  
    final java.util.Iterator<EffectRun> iterator = _effectsRuns.iterator();
    while (iterator.hasNext()) {
      final EffectRun effectRun = iterator.next();
      if (effectRun._started) {
        effectRun._effect.cancel(now);
      }
      effectRun.dispose();
      iterator.remove();
    }
  }

  public final void cancelAllEffectsFor(EffectTarget target)
  {
    final TimeInterval now = _timer.now();
  
    final java.util.Iterator<EffectRun> iterator = _effectsRuns.iterator();
    while (iterator.hasNext()) {
      final EffectRun effectRun = iterator.next();
      if (effectRun._target == target) {
        if (effectRun._started) {
          effectRun._effect.cancel(now);
        }
        effectRun.dispose();
        iterator.remove();
      }
    }
  }

  public final void onResume(G3MContext context)
  {

  }

  public final void onPause(G3MContext context)
  {

  }

  public final void onDestroy(G3MContext context)
  {

  }

}