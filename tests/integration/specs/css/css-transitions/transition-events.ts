describe('Transition events', () => {
  it('basic transitionrun', (done) => {
    const container1 = document.createElement('div');
    document.body.appendChild(container1);
    container1.addEventListener('transitionrun', async () => {
      await sleep(0.1);
      done();
    });
    setStyle(container1, {
      position: 'absolute',
      top: 0,
      left: 0,
      padding: '20px',
      backgroundColor: '#999',
      transitionProperty: 'transform',
      transitionDuration: '1s',
      transitionTimingFunction: 'linear',
    });
  });

  it('basic transitionstart', (done) => {
    const container1 = document.createElement('div');
    document.body.appendChild(container1);
    container1.addEventListener('transitionstart', async () => {
      done();
    });

    setStyle(container1, {
      position: 'absolute',
      top: 0,
      left: 0,
      padding: '20px',
      backgroundColor: '#999',
      transition: 'transform 1s linear',
    });

    requestAnimationFrame(async () => {
      setStyle(container1, {
        transform: 'translate(10px,20px)',
      });
    });
  });

  it('basic transitionend', (done) => {
    const container1 = document.createElement('div');
    document.body.appendChild(container1);
    setStyle(container1, {
      position: 'absolute',
      top: 0,
      left: 0,
      padding: '20px',
      backgroundColor: '#999',
      transition: 'transform 1s linear',
    });
    container1.appendChild(document.createTextNode('DIV 1'));

    container1.addEventListener('transitionend', async function self() {
      container1.removeEventListener('transitionend', self);
      await matchScreenshot();
      done();
    });
    requestAnimationFrame(async () => {
      await sleep(0.1);
      await matchScreenshot();
      setStyle(container1, {
        transform: 'translate(10px,20px)',
      });
    });
  });

  it('mutiple transitionend', (done) => {
    const container1 = document.createElement('div');
    document.body.appendChild(container1);
    setStyle(container1, {
      position: 'absolute',
      top: 0,
      left: 0,
      padding: '20px',
      backgroundColor: '#999',
      transition: 'transform 1s linear',
    });
    container1.appendChild(document.createTextNode('DIV 1'));

    function first() {
      container1.removeEventListener('transitionend', first);
      sleep(0.1)
        .then(() => {
          container1.addEventListener('transitionend', second);
          setStyle(container1, {
            transform: 'translate(250px,250px)',
          });
        });
    }

    function second() {
      container1.removeEventListener('transitionend', second);
      matchScreenshot().then(done);
    }

    container1.addEventListener('transitionend', first);
    requestAnimationFrame(async () => {
      await matchScreenshot();
      setStyle(container1, {
        transform: 'translate(30px,30px)',
      });
    });
  });
});