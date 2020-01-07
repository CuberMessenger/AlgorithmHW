using System;
using System.Buffers.Binary;
using System.IO;

namespace SimulateAnnealingFCNN {
    class Program {
        private static float[,,] TrainData { get; set; }
        private static int[] TrainLabel { get; set; }
        private static float[,,] TestData { get; set; }
        private static int[] TestLabel { get; set; }

        private static void ParseData(Stream trainDataStream, Stream trainLabelStream, Stream testDataStream, Stream testLabelStream) {
            if (trainDataStream != null) {
                using (BinaryReader reader = new BinaryReader(trainDataStream)) {
                    byte[] buffer = new byte[4];
                    trainDataStream.Read(buffer, 0, 4);
                    trainDataStream.Read(buffer, 0, 4);
                    int numOfImage = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    trainDataStream.Read(buffer, 0, 4);
                    int numOfRow = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    trainDataStream.Read(buffer, 0, 4);
                    int numOfColumn = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    TrainData = new float[numOfImage, numOfRow, numOfColumn];
                    for (int i = 0; i < numOfImage; i++) {
                        for (int r = 0; r < numOfRow; r++) {
                            for (int c = 0; c < numOfColumn; c++) {
                                TrainData[i, r, c] = reader.ReadByte() / 255f;
                            }
                        }
                    }
                }
            }

            if (trainLabelStream != null) {
                using (BinaryReader reader = new BinaryReader(trainLabelStream)) {
                    byte[] buffer = new byte[4];
                    trainLabelStream.Read(buffer, 0, 4);
                    trainLabelStream.Read(buffer, 0, 4);
                    int numOfImage = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    TrainLabel = new int[numOfImage];
                    for (int i = 0; i < numOfImage; i++) {
                        TrainLabel[i] = reader.ReadByte();
                    }
                }
            }

            if (testDataStream != null) {
                using (BinaryReader reader = new BinaryReader(testDataStream)) {
                    byte[] buffer = new byte[4];
                    testDataStream.Read(buffer, 0, 4);
                    testDataStream.Read(buffer, 0, 4);
                    int numOfImage = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    testDataStream.Read(buffer, 0, 4);
                    int numOfRow = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    testDataStream.Read(buffer, 0, 4);
                    int numOfColumn = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    TestData = new float[numOfImage, numOfRow, numOfColumn];
                    for (int i = 0; i < numOfImage; i++) {
                        for (int r = 0; r < numOfRow; r++) {
                            for (int c = 0; c < numOfColumn; c++) {
                                TestData[i, r, c] = reader.ReadByte() / 255f;
                            }
                        }
                    }
                }
            }

            if (testLabelStream != null) {
                using (BinaryReader reader = new BinaryReader(testLabelStream)) {
                    byte[] buffer = new byte[4];
                    testLabelStream.Read(buffer, 0, 4);
                    testLabelStream.Read(buffer, 0, 4);
                    int numOfImage = BinaryPrimitives.ReadInt32BigEndian(buffer);
                    TestLabel = new int[numOfImage];
                    for (int i = 0; i < numOfImage; i++) {
                        TestLabel[i] = reader.ReadByte();
                    }
                }
            }
        }

        static void Main(string[] args) {
            var trainDataStream = new FileStream(@"D:\Document\Workplace\AlgorithmHW\SimulateAnnealingFCNN\MNISTDataset\train-images.idx3-ubyte", FileMode.Open);
            var trainLabelStream = new FileStream(@"D:\Document\Workplace\AlgorithmHW\SimulateAnnealingFCNN\MNISTDataset\train-labels.idx1-ubyte", FileMode.Open);
            var testDataStream = new FileStream(@"D:\Document\Workplace\AlgorithmHW\SimulateAnnealingFCNN\MNISTDataset\t10k-images.idx3-ubyte", FileMode.Open);
            var testLabelStream = new FileStream(@"D:\Document\Workplace\AlgorithmHW\SimulateAnnealingFCNN\MNISTDataset\t10k-labels.idx1-ubyte", FileMode.Open);

            ParseData(trainDataStream, trainLabelStream, testDataStream, testLabelStream);

            //BackPropagationNeuralNetwork normalBPNN = new BackPropagationNeuralNetwork(new int[] { 28 * 28, 8, 10 }, epoch: 5, learnRate: 0.01f);
            //normalBPNN.Train(TrainData, TrainLabel);
            //Console.WriteLine("Train Done!");
            //Console.WriteLine($"Test Accuracy -> {normalBPNN.Test(TestData, TestLabel)}");

            BackPropagationNeuralNetwork simulateAnnealingBPNN = new BackPropagationNeuralNetwork(new int[] { 28 * 28, 16, 10 }, epoch: 5, learnRate: 0.01f);
            simulateAnnealingBPNN.Train(TrainData, TrainLabel);
            Console.WriteLine($"Test Accuracy -> {simulateAnnealingBPNN.Test(TestData, TestLabel)}");
            simulateAnnealingBPNN.TrainBySimulateAnnealing(TrainData, TrainLabel, TestData, TestLabel);
            Console.WriteLine("Train Done!");
            Console.WriteLine($"Test Accuracy -> {simulateAnnealingBPNN.Test(TestData, TestLabel)}");


        }
    }
}
