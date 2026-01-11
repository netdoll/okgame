import { Application, Ticker } from 'pixi.js';
import { EventEmitter } from 'eventemitter3';

export interface GameEvents {
  'scene:change': (sceneName: string) => void;
  'game:pause': () => void;
  'game:resume': () => void;
}

export class Game extends EventEmitter<GameEvents> {
  private app: Application;
  private isRunning: boolean = false;
  private isPaused: boolean = false;

  constructor(app: Application) {
    super();
    this.app = app;
  }

  async init(): Promise<void> {
    console.log('Game initializing...');
    this.app.ticker.add(this.update, this);
    this.app.ticker.stop();
    console.log('Game initialized');
  }

  start(): void {
    if (this.isRunning) return;
    console.log('Game starting...');
    this.isRunning = true;
    this.app.ticker.start();
  }

  stop(): void {
    if (!this.isRunning) return;
    console.log('Game stopping...');
    this.isRunning = false;
    this.app.ticker.stop();
  }

  pause(): void {
    if (this.isPaused) return;
    this.isPaused = true;
    this.emit('game:pause');
  }

  resume(): void {
    if (!this.isPaused) return;
    this.isPaused = false;
    this.emit('game:resume');
  }

  private update(ticker: Ticker): void {
    if (this.isPaused) return;
    const deltaTimeSeconds = ticker.deltaMS / 1000;
  }

  get width(): number {
    return this.app.screen.width;
  }

  get height(): number {
    return this.app.screen.height;
  }

  get stage() {
    return this.app.stage;
  }

  get renderer() {
    return this.app.renderer;
  }
}
