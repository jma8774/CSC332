// Jia Ming Ma 
class MultiThread implements Runnable {

    private int start;
    private int end;
    private int curMax;
    private int curCounter;
    private int threadNum;
    private int totalThreads;
    private long totalRunningTime;

    public MultiThread(int start, int end, int totalThreads) {
        this.start = start;
        this.end = end;
        this.curMax = 0; // current number with most divisor
        this.curCounter = 0; // curMax's amount of divisors
        this.threadNum = 1; // initial thread number starts at 1
        this.totalThreads = totalThreads; // total number of threads that should be used to split the work to
        this.totalRunningTime = 0; // total running time of all threads combined
    }

    public static void main(final String args[]) throws InterruptedException {
        int start = 1;
        int end = 100000;

        Runnable single = new MultiThread(start, end, 1);
        ((MultiThread) single).startThreads();
        System.out.println("Single thread took " + ((MultiThread) single).getTime() + " ms and " + ((MultiThread) single).getCurMax() + " has " + ((MultiThread) single).getCurCounter() + " divisors\n");

        Runnable duo = new MultiThread(start, end, 2);
        ((MultiThread) duo).startThreads();
        System.out.println("Double threads took " + ((MultiThread) duo).getTime() + " ms and " + ((MultiThread) duo).getCurMax() + " has " + ((MultiThread) duo).getCurCounter() + " divisors\n");

        Runnable quad = new MultiThread(start, end, 4);
        ((MultiThread) quad).startThreads();
        System.out.println("Quad threads took " + ((MultiThread) quad).getTime() + " ms and " + ((MultiThread) quad).getCurMax() + " has " + ((MultiThread) quad).getCurCounter() + " divisors\n");

        Runnable octo = new MultiThread(start, end, 8);
        ((MultiThread) octo).startThreads();
        System.out.println("Octo threads took " + ((MultiThread) octo).getTime() + " ms and " + ((MultiThread) octo).getCurMax() + " has " + ((MultiThread) octo).getCurCounter() + " divisors\n");

        Runnable twelve = new MultiThread(start, end, 12);
        ((MultiThread) twelve).startThreads();
        System.out.println("Twelve threads took " + ((MultiThread) twelve).getTime() + " ms and " + ((MultiThread) twelve).getCurMax() + " has " + ((MultiThread) twelve).getCurCounter() + " divisors\n");

        Runnable sixteen = new MultiThread(start, end, 16);
        ((MultiThread) sixteen).startThreads();
        System.out.println("Sixteen threads took " + ((MultiThread) sixteen).getTime() + " ms and " + ((MultiThread) sixteen).getCurMax() + " has " + ((MultiThread) sixteen).getCurCounter() + " divisors\n");
    } 

    public void startThreads() throws InterruptedException {
        long startTime = System.nanoTime(); // start time of threads execution

        Thread threads[] = new Thread[this.totalThreads]; // executing all the threads and splitting the work
        for(int i = 1; i <= this.totalThreads; i ++) {
            threads[i-1] = new Thread(this);
            threads[i-1].start();
        }
        for(int i = 1; i <= this.totalThreads; i ++) {
            threads[i-1].join(); // waits for all the threads to end befeore proceeding in main
        }

        long endTime   = System.nanoTime(); // end time
        long totalTime =  (endTime - startTime) / (long) Math.pow(10, 6); // divide it to convert from ns to ms
        this.totalRunningTime += totalTime;
    }

    public int getCurMax() {
        return this.curMax;
    }

    public int getCurCounter() {
        return this.curCounter;
    }

    public long getTime() {
        return this.totalRunningTime;
    }

    @Override
    public void run() {
        findHighestDivisorFrom(this.start, this.end, this.curMax, this.curCounter, this.threadNum, this.totalThreads);
    }

    public static int getDivisor(int n) {
        int start = 1;
        int counter = 0;
        while(start <= n){
            if(n % start == 0) {
                counter += 1;
            }
            start = start + 1;
        }
        return counter;
    }

    public void findHighestDivisorFrom(int start, int end, int curMax, int curCounter, final int threadNum, int totalThreads) {
        int segment = end / totalThreads; // determining segment I can seperate the work between threads
        if(threadNum != 1) start = segment * (threadNum - 1); // each thread starts at a diff location in our range
        if(threadNum != totalThreads) end = segment * threadNum; // account for round off error
        this.threadNum += 1;
        System.out.println("Thread " + threadNum + ": from " + start + " to " + end);
        for(int i = start; i < end; i++) {
            int numDiv = getDivisor(i);
            if(numDiv >= this.curCounter) {
                this.curMax = i;
                this.curCounter = numDiv;
            }
        }
    }
}