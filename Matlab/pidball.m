% Reglerkonstanten
Kp = 1.04;
Ki = 0.14;
Kd = 0.56;

s = tf('s');
Uebertragungsfunktion = 7.0071/((0.0016 * s^2 + 0.048 * s + 1) * s^2);

C = pid(Kp, Ki, Kd);
T = feedback(C * Uebertragungsfunktion, 1);

% Anzeigen der Sprungantwort
opt = stepDataOptions('InputOffset', 0,'StepAmplitude', 100);
t = 0:0.01:2.5;
step(T, t, opt);

% Konfiguration der Reglerkonstanten
%pidTuner(Uebertragungsfunktion, C);
