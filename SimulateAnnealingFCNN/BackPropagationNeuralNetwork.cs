using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimulateAnnealingFCNN {
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

        private static Random Random = new Random(System.DateTime.Now.Millisecond);

        private static float RandomFloat() => (float)Random.NextDouble() * 2f - 1f;

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
                Outputs[n] = Biases[n];
                for (int i = 0; i < NumOfWeights; i++) {
                    Outputs[n] += Weights[n, i] * PreviousLayer.Outputs[i];
                }
                Outputs[n] = ActivateFunction(Outputs[n]);
            }
        }

        internal void Backward() {
            for (int n = 0; n < NumOfNodes; n++) {
                Errors[n] = 0f;
                for (int i = 0; i < NextLayer.NumOfNodes; i++) {
                    Errors[n] += NextLayer.Errors[i] * NextLayer.Weights[i, n];
                }
                Errors[n] *= ActivationDerivative(Outputs[n]);

                Biases[n] -= CurrentNetwork.LearnRate * Errors[n];
                for (int i = 0; i < NumOfWeights; i++) {
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
            float[] delta = null;
            //int numOfInstance = 10000;
            int numOfInstance = trainData.GetLength(0);
            for (int e = 0; e < Epoch; e++) {
                for (int instanceIndex = 0; instanceIndex < numOfInstance; instanceIndex++) {
                    SetInput(trainData, instanceIndex, Layers.First());

                    //Forward
                    for (int i = 1; i < Layers.Length; i++) {
                        Layers[i].Forward();
                    }

                    var loss = LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]);
                    Losses[e] += loss.Average();

                    delta = LossFunctionDerivative(Layers.Last().Outputs, trainLabel[instanceIndex]);

                    //Backward
                    Layers.Last().OutputLayerBackward(delta);
                    for (int i = Layers.Length - 2; i > 0; i--) {
                        Layers[i].Backward();
                    }

                    if (instanceIndex % 1000 == 0) {
                        Console.WriteLine($"Epoch {e}, Loss -> {Losses[e] / (instanceIndex + 1)}");
                    }
                }
                LearnRate *= 0.8f;
            }
        }

        internal void TrainBySimulateAnnealing(float[,,] trainData, int[] trainLabel, float[,,] testData, int[] testLabel) {
            int numOfBatch = 100;
            int numOfInstance = trainData.GetLength(0);
            double Temperature = 1d;
            double loss = 0d, newLoss = 0d;
            float[] deltaW;
            float[] deltaB;
            int n, l = 1;
            (float[], float[,])[] bestWeight = null;
            double bestLoss = double.MaxValue;

            double EvaluateBatch(ref int instanceIndex) {
                double answer = 0d;
                for (int x = 0; x < numOfBatch; x++, instanceIndex = (instanceIndex + 1) % numOfInstance) {
                    SetInput(trainData, instanceIndex, Layers.First());
                    for (int i = 1; i < Layers.Length; i++) {
                        Layers[i].Forward();
                    }
                    answer += LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]).Average();
                }
                answer /= numOfBatch;
                return answer;
            }

            void FindNeighbour() {
                n = Random.Next(0, Layers[l].NumOfNodes);
                deltaW = new float[Layers[l].NumOfWeights];
                deltaB = new float[Layers[l].NumOfNodes];
                for (int i = 0; i < deltaW.Length; i++) {
                    deltaW[i] = Random.NextDouble() > 0.5d ? 0.01f : -0.01f;
                    Layers[l].Weights[n, i] += deltaW[i];
                }
                for (int i = 0; i < deltaB.Length; i++) {
                    deltaB[i] = Random.NextDouble() > 0.5d ? 0.01f : -0.01f;
                    Layers[l].Biases[i] += deltaB[i];
                }
            }

            void GoBackFromNeighbour() {
                for (int i = 0; i < deltaW.Length; i++) {
                    Layers[l].Weights[n, i] -= deltaW[i];
                }
                for (int i = 0; i < deltaB.Length; i++) {
                    Layers[l].Biases[i] -= deltaB[i];
                }
            }

            int instanceIndex = 0;
            while (Temperature > 1e-4) {
                for (int t = 0; t < 10; t++) {
                    loss = EvaluateBatch(ref instanceIndex);
                    if (loss < bestLoss) {
                        bestLoss = loss;
                        bestWeight = SaveWeights();
                        float acc = Test(testData, testLabel);
                        Console.WriteLine("Loss -> {0}\tTemperature -> {1}\tAccuracy -> {2}", Math.Round(loss, 3), Temperature, acc);
                    }

                    instanceIndex -= numOfBatch;
                    if (instanceIndex < 0) {
                        instanceIndex += numOfInstance;
                    }

                    FindNeighbour();

                    newLoss = EvaluateBatch(ref instanceIndex);

                    double deltaLoss = newLoss - loss;
                    if (deltaLoss >= 0 && (Random.NextDouble() >= Math.Exp(-(deltaLoss / Temperature)))) {
                        GoBackFromNeighbour();
                    }
                }
                if (instanceIndex == 0) {
                }
                Temperature *= 0.99d;
            }

            LoadWeights(bestWeight);
        }

        //internal void TrainBySimulateAnnealing(float[,,] trainData, int[] trainLabel) {
        //    int numOfBatch = 200;
        //    int numOfInstance = trainData.GetLength(0);
        //    double Temperature = 1d;
        //    double loss = 0d, newLoss = 0d;
        //    float[] deltaW;
        //    float[] deltaB;
        //    int n, l;

        //    double EvaluateBatch(ref int instanceIndex) {
        //        double answer = 0d;
        //        for (int x = 0; x < numOfBatch; x++, instanceIndex = (instanceIndex + 1) % numOfInstance) {
        //            SetInput(trainData, instanceIndex, Layers.First());
        //            for (int i = 1; i < Layers.Length; i++) {
        //                Layers[i].Forward();
        //            }
        //            answer += LossFunction(Layers.Last().Outputs, trainLabel[instanceIndex]).Average();
        //        }
        //        answer /= numOfBatch;
        //        return answer;
        //    }

        //    void FindNeighbour() {
        //        l = Random.Next(1, Layers.Length);
        //        n = Random.Next(0, Layers[l].NumOfNodes);
        //        deltaW = new float[Layers[l].NumOfWeights];
        //        deltaB = new float[Layers[l].NumOfNodes];
        //        for (int i = 0; i < deltaW.Length; i++) {
        //            deltaW[i] = Random.NextDouble() > 0.5d ? 0.01f : -0.01f;
        //            Layers[l].Weights[n, i] += deltaW[i];
        //        }
        //        for (int i = 0; i < deltaB.Length; i++) {
        //            deltaB[i] = Random.NextDouble() > 0.5d ? 0.01f : -0.01f;
        //            Layers[l].Biases[i] += deltaB[i];
        //        }
        //    }

        //    void GoBackFromNeighbour() {
        //        for (int i = 0; i < deltaW.Length; i++) {
        //            Layers[l].Weights[n, i] -= deltaW[i];
        //        }
        //        for (int i = 0; i < deltaB.Length; i++) {
        //            Layers[l].Biases[i] -= deltaB[i];
        //        }
        //    }

        //    int instanceIndex = 0;
        //    while (Temperature > 1e-7) {
        //        for (int t = 0; t < 10; t++) {
        //            loss = EvaluateBatch(ref instanceIndex);

        //            instanceIndex -= numOfBatch;
        //            if (instanceIndex < 0) {
        //                instanceIndex += numOfInstance;
        //            }

        //            FindNeighbour();

        //            newLoss = EvaluateBatch(ref instanceIndex);

        //            double deltaLoss = newLoss - loss;
        //            if (deltaLoss >= 0 && (Random.NextDouble() >= Math.Exp(-(deltaLoss / Temperature)))) {
        //                GoBackFromNeighbour();
        //            }
        //        }
        //        if (instanceIndex == 0) {
        //            Console.WriteLine("Loss -> {0}\tTemperature -> {1}", Math.Round(loss, 3), Temperature, 6);
        //        }
        //        Temperature *= 0.99d;
        //    }


        //}

        internal float Test(float[,,] testData, int[] testLabel) {
            float accuracy = 0f;
            int numOfInstance = testData.GetLength(0);
            for (int instanceIndex = 0; instanceIndex < numOfInstance; instanceIndex++) {
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
