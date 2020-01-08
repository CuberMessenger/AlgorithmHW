using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimulateAnnealingBPNN {
    class FullyConnectedNeuralLayer {
        internal int NumOfNodes { get; set; }
        internal int NumOfWeights { get; set; }
        internal float[] Outputs { get; set; }
        internal float[] Biases { get; set; }
        internal float[] Errors { get; set; }
        internal float[,] Weights { get; set; }
        internal BackPropagationNeuralNetwork CurrentNetwork { get; set; }
        internal FullyConnectedNeuralLayer PreviousLayer { get; set; }
        internal FullyConnectedNeuralLayer NextLayer { get; set; }
        internal Func<float, float> ActivateFunction { get; set; }
        internal Func<float, float> ActivationDerivative { get; set; }

        internal static float ReLu(float x) => x > 0f ? x : 0f;
        internal static float ReLuDerivative(float relu) => relu > 0f ? 1f : 0f;
        internal static float Linear(float x) => x;
        internal static float LinearDerivative(float linear) => 1f;

        internal static float SoftmaxShift { get; set; }
        internal static void SoftmaxInPlace(float[] x) {
            SoftmaxShift = x.Max();
            for (int i = 0; i < x.Length; i++) {
                x[i] = (float)Math.Exp(x[i] - SoftmaxShift);
            }
            float sum = x.Sum();
            for (int i = 0; i < x.Length; i++) {
                x[i] /= sum;
            }
        }

        internal static float SoftmaxDerivative(float softmax) => softmax * (1f - softmax);

        internal static float Sigmoid(float x) => 1f / (1f + (float)Math.Exp(-x));

        internal static float SigmoidDerivative(float sigmoid) => sigmoid * (1f - sigmoid);

        private Random Random = new Random(69);

        private float RandomFloat() => (float)Random.NextDouble() * 2f - 1f;

        internal FullyConnectedNeuralLayer(int numOfNodes, int numOfWeights, BackPropagationNeuralNetwork currentNetwork) {
            NumOfNodes = numOfNodes;
            NumOfWeights = numOfWeights;
            Outputs = new float[numOfNodes];
            Biases = new float[numOfNodes];
            Errors = new float[numOfNodes];
            Weights = new float[numOfNodes, numOfWeights];
            for (int i = 0; i < numOfNodes; i++) {
                Outputs[i] = RandomFloat();
                Biases[i] = RandomFloat();
                Errors[i] = RandomFloat();
                for (int j = 0; j < numOfWeights; j++) {
                    Weights[i, j] = RandomFloat();
                }
            }
            CurrentNetwork = currentNetwork;
            PreviousLayer = null;
            NextLayer = null;
            ActivateFunction = Sigmoid;
            ActivationDerivative = SigmoidDerivative;
        }

        internal void Forward() {
            for (int n = 0; n < NumOfNodes; n++) {
                //加上偏置
                Outputs[n] = Biases[n];
                //加上前一层节点的输出与权重的乘积
                for (int i = 0; i < NumOfWeights; i++) {
                    Outputs[n] += Weights[n, i] * PreviousLayer.Outputs[i];
                }
                //经过激活函数
                Outputs[n] = ActivateFunction(Outputs[n]);
            }
        }

        internal void Backward() {
            for (int n = 0; n < NumOfNodes; n++) {
                Errors[n] = 0f;
                //计算梯度，为后一层用到了当前节点的输出的节点的梯度乘对应权重的和
                for (int i = 0; i < NextLayer.NumOfNodes; i++) {
                    Errors[n] += NextLayer.Errors[i] * NextLayer.Weights[i, n];
                }
                //计算到激活函数前的梯度
                Errors[n] *= ActivationDerivative(Outputs[n]);

                //根据梯度更新偏置
                Biases[n] -= CurrentNetwork.LearnRate * Errors[n];
                for (int i = 0; i < NumOfWeights; i++) {
                    //根据梯度更新权重
                    Weights[n, i] -= CurrentNetwork.LearnRate * Errors[n] * PreviousLayer.Outputs[i];
                }
            }
        }

        internal void OutputLayerBackward(float[] delta) {
            for (int n = 0; n < NumOfNodes; n++) {
                Errors[n] = delta[n] * SigmoidDerivative(Outputs[n]);

                Biases[n] -= CurrentNetwork.LearnRate * Errors[n];
                for (int i = 0; i < NumOfWeights; i++) {
                    Weights[n, i] -= CurrentNetwork.LearnRate * Errors[n] * PreviousLayer.Outputs[i];
                }
            }
        }
    }

    class BackPropagationNeuralNetwork {
        internal int Epoch { get; set; }
        internal float LearnRate { get; set; }
        internal float[] Losses { get; set; }
        internal FullyConnectedNeuralLayer[] Layers { get; set; }
        internal Func<float[], int, float[]> LossFunction { get; set; }
        internal Func<float[], int, float[]> LossFunctionDerivative { get; set; }

        internal float[] LossCurve { get; set; }

        private static Random Random = new Random(System.DateTime.Now.Millisecond);

        internal static float[] CrossEntropy(float[] outputs, int target) {
            float[] answer = new float[outputs.Length];
            for (int i = 0; i < outputs.Length; i++) {
                answer[i] = (float)(target == i ? -Math.Log(outputs[i] + 0.0001f) : -Math.Log(1f - outputs[i] + 0.0001f));
            }
            return answer;
        }

        internal static float[] CrossEntropyDerivative(float[] outputs, int target) {
            float[] answer = new float[outputs.Length];
            for (int i = 0; i < answer.Length; i++) {
                if (target == i) {
                    answer[i] = -1f / (outputs[i] + 0.0001f);
                }
                else {
                    answer[i] = 1f / (1f - outputs[i] + 0.0001f);
                }
            }
            return answer;
        }

        internal BackPropagationNeuralNetwork(int[] networkShape, int epoch, float learnRate) {
            LossCurve = new float[600];
            Epoch = epoch;
            LearnRate = learnRate;
            Losses = new float[epoch];
            for (int i = 0; i < epoch; i++) {
                Losses[i] = 0f;
            }

            LossFunction = CrossEntropy;
            LossFunctionDerivative = CrossEntropyDerivative;

            Layers = new FullyConnectedNeuralLayer[networkShape.Length];
            Layers[0] = new FullyConnectedNeuralLayer(networkShape[0], 0, this);
            for (int i = 1; i < networkShape.Length - 1; i++) {
                Layers[i] = new FullyConnectedNeuralLayer(networkShape[i], networkShape[i - 1], this);
            }
            Layers[Layers.Length - 1] = new FullyConnectedNeuralLayer(networkShape[Layers.Length - 1], networkShape[Layers.Length - 2], this);

            Layers[0].NextLayer = Layers[1];
            Layers[Layers.Length - 1].PreviousLayer = Layers[Layers.Length - 2];
            for (int i = 1; i < Layers.Length - 1; i++) {
                Layers[i].PreviousLayer = Layers[i - 1];
                Layers[i].NextLayer = Layers[i + 1];
            }
        }

        internal (float[], float[,])[] SaveWeights() {
            (float[], float[,])[] answer = new (float[], float[,])[Layers.Length - 1];
            for (int i = 1; i < Layers.Length; i++) {
                float[] biasesTemp = new float[Layers[i].NumOfNodes];
                float[,] weightsTemp = new float[Layers[i].NumOfNodes, Layers[i].NumOfWeights];
                for (int n = 0; n < Layers[i].NumOfNodes; n++) {
                    biasesTemp[n] = Layers[i].Biases[n];
                    for (int w = 0; w < Layers[i].NumOfWeights; w++) {
                        weightsTemp[n, w] = Layers[i].Weights[n, w];
                    }
                }
                answer[i - 1] = (biasesTemp, weightsTemp);
            }
            return answer;
        }

        internal void LoadWeights((float[], float[,])[] weight) {
            for (int i = 1; i < Layers.Length; i++) {
                float[] biasesTemp = weight[i - 1].Item1;
                float[,] weightsTemp = weight[i - 1].Item2;
                for (int n = 0; n < Layers[i].NumOfNodes; n++) {
                    Layers[i].Biases[n] = biasesTemp[n];
                    for (int w = 0; w < Layers[i].NumOfWeights; w++) {
                        Layers[i].Weights[n, w] = weightsTemp[n, w];
                    }
                }
            }
        }

        internal void SetInput(float[,,] data, int instanceIndex, FullyConnectedNeuralLayer layer) {
            int sideLength = data.GetLength(1);
            for (int i = 0, r = 0, c = 0; i < layer.NumOfNodes; i++) {
                layer.Outputs[i] = data[instanceIndex, r, c];
                c++;
                if (c == sideLength) {
                    c = 0;
                    r++;
                }
            }
        }

        internal void Train(float[,,] trainData, int[] trainLabel) {
            int noi = 0, noc = 0;
            float[] delta = null;
            //int numOfInstance = 50000;
            int numOfInstance = trainData.GetLength(0);
            for (int e = 0; e < Epoch; e++) {
                for (int instanceIndex = 0; instanceIndex < numOfInstance; instanceIndex++) {
                    SetInput(trainData, instanceIndex, Layers.First());

                    //Forward
                    for (int i = 1; i < Layers.Length; i++) {
                        Layers[i].Forward();
                    }

                    var loss = LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]).Average();
                    Losses[0] += loss;

                    delta = LossFunctionDerivative(Layers.Last().Outputs, trainLabel[instanceIndex]);

                    //Backward
                    Layers.Last().OutputLayerBackward(delta);
                    for (int i = Layers.Length - 2; i > 0; i--) {
                        Layers[i].Backward();
                    }

                    if (instanceIndex % 1000 == 0) {
                        Console.WriteLine($"Epoch {e}, Loss -> {Losses[0] / (noi + 1)}");
                        LossCurve[noc++] = Losses[0] / (noi + 1);
                        noi += 1000;
                    }
                }
                LearnRate *= 0.8f;
            }
        }

        internal void TrainBySimulateAnnealing(float[,,] trainData, int[] trainLabel) {
            double Temperature = 0.1d;
            int noi = 0, noc = 0;
            float loss, newLoss, deltaLoss;
            float[] delta = null;
            //int numOfInstance = 50000;
            int numOfInstance = trainData.GetLength(0);
            for (int e = 0; e < Epoch; e++) {
                for (int instanceIndex = 0; instanceIndex < numOfInstance; instanceIndex++) {
                    SetInput(trainData, instanceIndex, Layers.First());
                    for (int i = 1; i < Layers.Length; i++) {
                        Layers[i].Forward();
                    }

                    loss = LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]).Average();
                    Losses[0] += loss;

                    delta = LossFunctionDerivative(Layers.Last().Outputs, trainLabel[instanceIndex]);

                    //Backward
                    Layers.Last().OutputLayerBackward(delta);
                    for (int i = Layers.Length - 2; i > 0; i--) {
                        Layers[i].Backward();
                    }
                    //SA
                    SetInput(trainData, instanceIndex, Layers.First());
                    for (int i = 1; i < Layers.Length; i++) {
                        Layers[i].Forward();
                    }

                    newLoss = LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]).Average();
                    deltaLoss = newLoss - loss;
                    if (deltaLoss >= 0 && (Random.NextDouble() >= Math.Exp(-(deltaLoss / Temperature)))) {
                        Console.WriteLine("!!!!!!!!!!!!!!!!!!!!");
                        int l = Random.Next(1, Layers.Length);
                        int n = Random.Next(0, Layers[l].NumOfNodes);
                        var deltaW = new float[Layers[l].NumOfWeights];
                        var deltaB = new float[Layers[l].NumOfNodes];
                        for (int i = 0; i < deltaW.Length; i++) {
                            deltaW[i] = Random.NextDouble() > 0.5d ? 0.1f : -0.1f;
                            Layers[l].Weights[n, i] += deltaW[i];
                        }
                        for (int i = 0; i < deltaB.Length; i++) {
                            deltaB[i] = Random.NextDouble() > 0.5d ? 0.1f : -0.1f;
                            Layers[l].Biases[i] += deltaB[i];
                        }
                    }

                    if (instanceIndex % 1000 == 0) {
                        Console.WriteLine($"Epoch {e}\tT -> {Math.Round(Temperature, 3)}\tLoss -> {Losses[0] / (noi + 1)}");
                        LossCurve[noc++] = Losses[0] / (noi + 1);
                        noi += 1000;
                        Temperature *= 0.99d;
                    }
                }
                LearnRate *= 0.8f;
            }
        }

        internal float Test(float[,,] testData, int[] testLabel, int startIndex = 0) {
            float accuracy = 0f;
            int numOfInstance = testData.GetLength(0);
            for (int instanceIndex = startIndex; instanceIndex < numOfInstance; instanceIndex++) {
                accuracy += TestOne(testData, instanceIndex) == testLabel[instanceIndex] ? 1f : 0f;
            }
            accuracy /= numOfInstance;
            return accuracy;
        }

        internal int TestOne(float[,,] testData, int instanceIndex) {
            SetInput(testData, instanceIndex, Layers.First());
            for (int i = 1; i < Layers.Length; i++) {
                Layers[i].Forward();
            }
            var output = Layers.Last().Outputs.ToList();
            return output.IndexOf(output.Max());
        }

        internal int TestOne(float[,] data) {
            var layer = Layers.First();
            int sideLength = data.GetLength(1);
            for (int i = 0, r = 0, c = 0; i < layer.NumOfNodes; i++) {
                layer.Outputs[i] = data[r, c];
                c++;
                if (c == sideLength) {
                    c = 0;
                    r++;
                }
            }

            for (int i = 1; i < Layers.Length; i++) {
                Layers[i].Forward();
            }
            var output = Layers.Last().Outputs.ToList();
            return output.IndexOf(output.Max());
        }
    }
}
