package com.aaomidi.jchat.chat.engine;

import com.aaomidi.jchat.chat.Main;
import com.aaomidi.jchat.chat.handlers.ChatServerHandler;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;

public class ChatServer {
    private final Main instance;
    private Channel channel;
    private EventLoopGroup bossGroup = new NioEventLoopGroup();
    private EventLoopGroup workerGroup = new NioEventLoopGroup();

    public ChatServer(Main instance) {
        this.instance = instance;
        try {
            this.create();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void create() throws Exception {
        bossGroup = new NioEventLoopGroup();
        workerGroup = new NioEventLoopGroup();

        ServerBootstrap b = new ServerBootstrap();
        b.group(bossGroup, workerGroup)
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    public void initChannel(SocketChannel ch) throws Exception {
                        ch.pipeline().addLast(new ChatServerHandler(instance));
                    }
                })
                .option(ChannelOption.SO_BACKLOG, 128)
                .childOption(ChannelOption.SO_KEEPALIVE, true);


        ChannelFuture f = b.bind(instance.getLoadedConfiguration().getBindNumber()).sync();


    }

    public void shutDown() {
        workerGroup.shutdownGracefully();
        bossGroup.shutdownGracefully();
    }
}
