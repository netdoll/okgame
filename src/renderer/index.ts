import { Application } from 'pixi.js';
import { Game } from './Game';

const isElectron = typeof window !== 'undefined' && window.electronAPI !== undefined;

async function main(): Promise<void> {
  console.log(`bob's game starting... (${isElectron ? 'Electron' : 'Browser'} mode)`);

  const app = new Application();
  
  await app.init({
    width: window.innerWidth,
    height: window.innerHeight,
    backgroundColor: 0x000000,
    resolution: window.devicePixelRatio || 1,
    autoDensity: true,
    antialias: true,
  });

  const container = document.getElementById('game-container');
  if (container) {
    container.appendChild(app.canvas);
  } else {
    document.body.appendChild(app.canvas);
  }

  window.addEventListener('resize', () => {
    app.renderer.resize(window.innerWidth, window.innerHeight);
  });

  const game = new Game(app);
  await game.init();
  game.start();
}

if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', main);
} else {
  main();
}
