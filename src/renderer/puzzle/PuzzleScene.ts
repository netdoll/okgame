import { Application } from 'pixi.js';
import { Scene, SceneConfig } from '../state/Scene';
import { InputManager, Key } from '../input/InputManager';
import { AudioManager } from '../audio/AudioManager';
import { PuzzleGame, GameState, MovementType } from './PuzzleGame';
import { PuzzleRenderer } from './PuzzleRenderer';
import { GameType, GameTypes } from './GameType';

export interface PuzzleSceneConfig extends SceneConfig {
  gameType?: GameType;
  startLevel?: number;
  seed?: number;
}

export interface PuzzleKeyBindings {
  left: Key | string;
  right: Key | string;
  down: Key | string;
  hardDrop: Key | string;
  rotateCW: Key | string;
  rotateCCW: Key | string;
  rotate180: Key | string;
  hold: Key | string;
  pause: Key | string;
  restart: Key | string;
}

const DEFAULT_BINDINGS: PuzzleKeyBindings = {
  left: Key.Left,
  right: Key.Right,
  down: Key.Down,
  hardDrop: Key.Space,
  rotateCW: Key.Up,
  rotateCCW: Key.Z,
  rotate180: Key.A,
  hold: Key.C,
  pause: Key.Escape,
  restart: Key.R,
};

export class PuzzleScene extends Scene {
  private game: PuzzleGame;
  private renderer: PuzzleRenderer;
  private bindings: PuzzleKeyBindings;

  private gameType: GameType;
  private startLevel: number;
  private seed?: number;

  private tickAccumulator: number = 0;
  private readonly TICK_RATE: number = 60;
  private readonly TICK_DURATION: number = 1000 / this.TICK_RATE;

  private soundsLoaded: boolean = false;

  constructor(config: PuzzleSceneConfig, bindings?: Partial<PuzzleKeyBindings>) {
    super(config);
    this.gameType = config.gameType ?? GameTypes.MODERN;
    this.startLevel = config.startLevel ?? 1;
    this.seed = config.seed;
    this.bindings = { ...DEFAULT_BINDINGS, ...bindings };

    this.game = new PuzzleGame({
      gameType: this.gameType,
      startLevel: this.startLevel,
      seed: this.seed,
    });

    this.renderer = new PuzzleRenderer({
      cellSize: 32,
      gridOffsetX: 200,
      gridOffsetY: 50,
    });
  }

  protected async create(): Promise<void> {
    this.renderer.attachGame(this.game);
    this.container.addChild(this.renderer.container);

    this.centerRenderer();
    this.loadSounds();
    this.setupGameEvents();

    this.game.init();
    this.game.start();
  }

  private centerRenderer(): void {
    const bounds = this.renderer.getGridBounds();
    const totalWidth = bounds.width + 400;
    const offsetX = (this.width - totalWidth) / 2;
    this.renderer.setPosition(offsetX, 0);
  }

  private loadSounds(): void {
    const sounds = [
      { name: 'puzzle_move', src: 'assets/audio/sfx/move.wav' },
      { name: 'puzzle_rotate', src: 'assets/audio/sfx/rotate.wav' },
      { name: 'puzzle_drop', src: 'assets/audio/sfx/drop.wav' },
      { name: 'puzzle_lock', src: 'assets/audio/sfx/lock.wav' },
      { name: 'puzzle_clear', src: 'assets/audio/sfx/clear.wav' },
      { name: 'puzzle_tetris', src: 'assets/audio/sfx/tetris.wav' },
      { name: 'puzzle_hold', src: 'assets/audio/sfx/hold.wav' },
      { name: 'puzzle_levelup', src: 'assets/audio/sfx/levelup.wav' },
      { name: 'puzzle_gameover', src: 'assets/audio/sfx/gameover.wav' },
    ];

    for (const sound of sounds) {
      try {
        AudioManager.load(sound.name, sound.src);
      } catch {
        // Intentionally empty - sounds are optional
      }
    }
    this.soundsLoaded = true;
  }

  private setupGameEvents(): void {
    this.game.on('pieceMoved', (_piece, movement) => {
      if (movement === MovementType.LEFT || movement === MovementType.RIGHT) {
        this.playSound('puzzle_move');
      } else if (
        movement === MovementType.ROTATE_CW ||
        movement === MovementType.ROTATE_CCW ||
        movement === MovementType.ROTATE_180
      ) {
        this.playSound('puzzle_rotate');
      } else if (movement === MovementType.HARD_DROP) {
        this.playSound('puzzle_drop');
      } else if (movement === MovementType.HOLD) {
        this.playSound('puzzle_hold');
      }
    });

    this.game.on('pieceLocked', () => {
      this.playSound('puzzle_lock');
    });

    this.game.on('linesCleared', (lines) => {
      if (lines.length >= 4) {
        this.playSound('puzzle_tetris');
      } else {
        this.playSound('puzzle_clear');
      }
    });

    this.game.on('levelUp', () => {
      this.playSound('puzzle_levelup');
    });

    this.game.on('gameOver', () => {
      this.playSound('puzzle_gameover');
    });
  }

  private playSound(name: string): void {
    if (this.soundsLoaded && AudioManager.isLoaded(name)) {
      AudioManager.playSound(name);
    }
  }

  protected onUpdate(dt: number): void {
    this.tickAccumulator += dt;

    while (this.tickAccumulator >= this.TICK_DURATION) {
      this.tickAccumulator -= this.TICK_DURATION;
      this.processInput();
      this.game.update();
    }

    this.renderer.update();
  }

  private processInput(): void {
    if (InputManager.isKeyPressed(this.bindings.pause)) {
      if (this.game.state === GameState.PLAYING) {
        this.game.pause();
      } else if (this.game.state === GameState.PAUSED) {
        this.game.resume();
      }
      return;
    }

    if (InputManager.isKeyPressed(this.bindings.restart)) {
      this.restart();
      return;
    }

    if (this.game.state !== GameState.PLAYING) return;

    this.game.setInput({
      left: InputManager.isKeyHeld(this.bindings.left),
      right: InputManager.isKeyHeld(this.bindings.right),
      down: InputManager.isKeyHeld(this.bindings.down),
      rotateCW: InputManager.isKeyPressed(this.bindings.rotateCW),
      rotateCCW: InputManager.isKeyPressed(this.bindings.rotateCCW),
      rotate180: InputManager.isKeyPressed(this.bindings.rotate180),
      hardDrop: InputManager.isKeyPressed(this.bindings.hardDrop),
      hold: InputManager.isKeyPressed(this.bindings.hold),
    });
  }

  restart(): void {
    this.game.init();
    this.game.start();
  }

  getGame(): PuzzleGame {
    return this.game;
  }

  getRenderer(): PuzzleRenderer {
    return this.renderer;
  }

  protected async destroy(): Promise<void> {
    this.game.removeAllListeners();
    this.renderer.destroy();
  }
}
