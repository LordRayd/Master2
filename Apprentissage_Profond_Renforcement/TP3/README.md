# Pavia
## Resultat
### Top 8572

model = Sequential()
#model.add(Flatten())
model.add(Dense(H, input_dim=N))
model.add(Activation("relu"))
model.add(Dense(300))
model.add(Activation("tanh"))
model.add(Dense(100))
model.add(Activation("relu"))
model.add(Dense(300))
model.add(Activation("tanh"))
model.add(Dense(K))
model.add(Activation("softmax"))
model.compile(optimizer=optimizers.Adadelta(learning_rate=0.1,  rho=0.99), loss='categorical_crossentropy', metrics=['accuracy'])

### Top 8539

```
model.add(Dense(H, input_dim=N))
model.add(Activation("relu"))
model.add(Dense(300))
model.add(Activation("tanh"))
model.add(Dense(300))
model.add(Activation("relu"))
model.add(Dense(200))
model.add(Activation("tanh"))
model.add(Dense(K))
model.add(Activation("softmax"))
Adadelta(learning_rate=0.1)
```
### Top 1 - 0.8469
```
model.add(Dense(H, input_dim=N))
model.add(Activation("relu"))
model.add(Dense(300))
model.add(Activation("tanh"))
model.add(Dense(300))
model.add(Activation("relu"))
model.add(Dense(100))
model.add(Activation("tanh"))
model.add(Dense(K))
model.add(Activation("softmax"))
Adadelta(learning_rate=0.08)
```