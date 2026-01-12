import { Application, Ticker, Container, Sprite as PIXISprite, Assets, Text, TextStyle } from 'pixi.js';
import { EventEmitter } from 'eventemitter3';
import { Camera } from './graphics/Camera';
import { StateManager } from './state/StateManager';
import { InputManager } from './input/InputManager';

export interface GameEvents {
  'scene:change': (sceneName: string) => void;
  'game:pause': () => void;
  'game:resume': () => void;
  'camera:zoom': (zoom: number) => void;
}

export class Game extends EventEmitter<GameEvents> {
  private app: Application;
  private isRunning: boolean = false;
  private isPaused: boolean = false;

  private _camera: Camera;
  private worldContainer: Container;
  private demoSprite: PIXISprite | null = null;

  constructor(app: Application) {
    super();
    this.app = app;

    this.worldContainer = new Container();
    this.app.stage.addChild(this.worldContainer);

    this._camera = new Camera(this.worldContainer, {
      viewportWidth: app.screen.width,
      viewportHeight: app.screen.height,
      defaultZoom: 2.0,
      minZoom: 0.5,
      maxZoom: 4.0,
    });
  }

  async init(): Promise<void> {
    console.log('Game initializing...');
    
    InputManager.init();
    
    this.app.ticker.add(this.update, this);
    this.app.ticker.stop();

    await this.loadDemo();
    this.setupControls();

    console.log('Game initialized');
  }

  private async loadDemo(): Promise<void> {
    try {
      const texture = await Assets.load('/theme/bobcorp.png');
      this.demoSprite = new PIXISprite(texture);
      this.demoSprite.anchor.set(0.5);
      this.demoSprite.position.set(0, 0);
      this.worldContainer.addChild(this.demoSprite);

      this._camera.centerOn(0, 0);

      const style = new TextStyle({
        fontFamily: 'Arial',
        fontSize: 14,
        fill: 0xffffff,
      });
      const helpText = new Text({
        text: 'Controls: Arrow keys = pan, +/- = zoom, Space = shake, R = reset',
        style,
      });
      helpText.position.set(10, 10);
      this.app.stage.addChild(helpText);

      console.log('Demo loaded: bobcorp.png');
    } catch (err) {
      console.error('Failed to load demo texture:', err);
    }
  }

  private setupControls(): void {
    const moveSpeed = 5;
    const keys = new Set<string>();

    window.addEventListener('keydown', (e) => {
      keys.add(e.key);

      if (e.key === '=' || e.key === '+') {
        this._camera.zoomIn();
      } else if (e.key === '-') {
        this._camera.zoomOut();
      } else if (e.key === ' ') {
        this._camera.setShake(500, 10, 10);
      } else if (e.key === 'r' || e.key === 'R') {
        this._camera.centerOn(0, 0);
        this._camera.zoom = 2.0;
      }
    });

    window.addEventListener('keyup', (e) => {
      keys.delete(e.key);
    });

    this.app.ticker.add(() => {
      if (keys.has('ArrowLeft')) this._camera.x -= moveSpeed;
      if (keys.has('ArrowRight')) this._camera.x += moveSpeed;
      if (keys.has('ArrowUp')) this._camera.y -= moveSpeed;
      if (keys.has('ArrowDown')) this._camera.y += moveSpeed;
    });
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
    const deltaMs = ticker.deltaMS;
    const dt = deltaMs / 1000;
    
    InputManager.update();
    StateManager.update(dt);
    
    this._camera.update(deltaMs);
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

  get camera(): Camera {
    return this._camera;
  }

  get world(): Container {
    return this.worldContainer;
  }

  resize(width: number, height: number): void {
    this.app.renderer.resize(width, height);
    this._camera.resize(width, height);
  }
}
